#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "../Helpers/BinaryReader.h"
#include "../Content/ContentManager.h"
#include "../Graphics/TextRenderer.h"
#include "TextureDataLoader.h"


SpriteFontLoader::SpriteFontLoader()
{

}


SpriteFontLoader::~SpriteFontLoader()
{
}

#pragma warning( disable : 4702 )
SpriteFont* SpriteFontLoader::LoadContent(const wstring& assetFile)
{
	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout
	
	//Parse the Identification bytes (B,M,F)
	char fileId1 = pBinReader->Read<char>();
	char fileId2 = pBinReader->Read<char>();
	char fileId3 = pBinReader->Read<char>();

	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	if(fileId1 != 'B' || fileId2 != 'M' || fileId3 != 'F')
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetfile! \nPath: \'%s\'", assetFile.c_str());
	
		return nullptr;
	}

	//Parse the version (version 3 required)
	int version = pBinReader->Read<char>();
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	if (version != 3)
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported! \nFile version: \'%i\'", version);

		return nullptr;
	}
	

	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	int blockId = pBinReader->Read<char>();
	int blockSize = pBinReader->Read<int>();

	//Retrieve the FontSize
	pSpriteFont->m_FontSize = pBinReader->Read<short>();
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pBinReader->MoveBufferPosition(12);
	//Retrieve the FontName [SpriteFont::m_FontName]
	pSpriteFont->m_FontName = pBinReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();

	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	pBinReader->MoveBufferPosition(4);
	pSpriteFont->m_TextureWidth = pBinReader->Read<unsigned short>();
	pSpriteFont->m_TextureHeight = pBinReader->Read<unsigned short>();
	//Retrieve PageCount
	int pageCount = pBinReader->Read<unsigned short>();
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	if (pageCount > 1)
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > SpriteFont\n Only one texture per font allowed.");

		return nullptr;
	}

	//Advance to Block2 (Move Reader)
	pBinReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	//Retrieve the PageName (store Local)
	wstring pageName = pBinReader->ReadNullString();
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	if (pageName.empty())
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > SpriteFont\n inavlid font sprite (empty).");

		return nullptr;
	}

	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	wstring filepath = assetFile.substr(0, assetFile.rfind('/')+1);
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(filepath + pageName);
	
	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	//Retrieve Character Count (see documentation)
	int charCount = blockSize / 20;
	//Retrieve Every Character, For every Character:
	for(int i = 0; i < charCount; ++i)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
		auto charId = wchar_t( pBinReader->Read<unsigned int>());
		if (!pSpriteFont->IsCharValid(charId))
		{
			Logger::LogFormat(LogLevel::Warning, L"SpriteFontLoader::LoadContent > SpriteFont\n inavlid character Id : \'%c\' .", charId);
			pBinReader->MoveBufferPosition(16);
		}
		else
		{
			//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
			//> Set IsValid to true [FontMetric::IsValid]
			//> Set Character (CharacterId) [FontMetric::Character]
			auto fontMetric = &pSpriteFont->GetMetric(charId);
			fontMetric->IsValid = true;

			//> Retrieve Xposition (store Local)
			auto xPos = pBinReader->Read<unsigned short>();
			//> Retrieve Yposition (store Local)
			auto yPos = pBinReader->Read<unsigned short>();
			//> Retrieve & Set Width [FontMetric::Width]
			fontMetric->Width = pBinReader->Read<unsigned short>();
			//> Retrieve & Set Height [FontMetric::Height]
			fontMetric->Height = pBinReader->Read<unsigned short>();
			//> Retrieve & Set OffsetX [FontMetric::OffsetX]
			fontMetric->OffsetX = pBinReader->Read<short>();
			//> Retrieve & Set OffsetY [FontMetric::OffsetY]
			fontMetric->OffsetY = pBinReader->Read<short>();
			//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
			fontMetric->AdvanceX = pBinReader->Read<short>();
			//> Retrieve & Set Page [FontMetric::Page]
			fontMetric->Page = pBinReader->Read<char>();
			//> Retrieve Channel (BITFIELD!!!) 
			//	> See documentation for BitField meaning [FontMetrix::Channel]

			auto chnl = pBinReader->Read<char>();
			switch (chnl)
			{
			case 1:fontMetric->Channel = 2; break;
			case 2:fontMetric->Channel = 1; break;
			case 4:fontMetric->Channel = 0; break;
			case 8:fontMetric->Channel = 3; break;
			default: fontMetric->Channel = 4; break;
			}
			//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
			XMFLOAT2 texture = { float(xPos) / pSpriteFont->m_TextureWidth , float(yPos) / pSpriteFont->m_TextureWidth };
			fontMetric->TexCoord = texture;
		}

	}
	//DONE :)

	delete pBinReader;
	return pSpriteFont;
	
	#pragma warning(default:4702)  
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}