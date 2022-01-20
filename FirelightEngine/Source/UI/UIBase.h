#pragma once
#include"..\ECS\Entity.h"
#include"..\Events\Listener.h"
#include"..\Graphics\Data\Texture.h"
#include"..\Maths\Vec2.h"
#include"..\Maths\Rect.h"
//This file will be removed 


namespace Firelight::UI {
	class UIBase:public Events::Listener
	{

		public:
			UIBase();
			~UIBase();
			void HandleEvents(void* data);
		private:

			//Event function Invoke
			void OnPress();
			void OnHover();
			void OnDrag();

		public:
		

		private:
			//Spritesheet
			//position 
	};

	
	//https://github.com/planetchili/Sprite/tree/master/Engine
	class TextRender
	{
	public:
		TextRender(const std::string& filename, Graphics::Colour chroma = Graphics::Colours::sc_defaultNormal);
		~TextRender();

		void DrawTextFromSpriteSheet(const std::string& text, const Maths::Vec2f pos, Graphics::Colour colour);
		

	private:
		Maths::Rectf MapGlyphRect(char c) const;

		Graphics::Texture SpriteTex;
		// this gives the dimensions of a glyph in the font sheet
		int glyphWidth;
		int glyphHeight;
		// number of rows / columns in the font sheet (this is fixed)
		static constexpr int nColumns = 32;
		static constexpr int nRows = 3;

		// font sheet chroma color
		Graphics::Colour chroma;
		// start and end drawable character codes
		static constexpr char firstChar = ' ';
		static constexpr char lastChar = '~';

	};

}
