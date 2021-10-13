/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include "CrystalRender.hpp"

/*
*	Render Class
*	Handles all rendering...
*/
namespace EliCrystal {
	/*
	*	Method to initalize Vita2D
	*/
	void CrystalRender::InitVita2d(){
		sceClibPrintf("Setting up Vita2D..\n");
		vita2d_init();
		vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
		
		this->TotalImages = -1;
		this->DrawColor = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
		this->DrawSize = 1.0F;
		this->DrawFont = CrystalPgfFont;
		this->Pgf = vita2d_load_default_pgf();
		this->Pvf = vita2d_load_default_pvf();
		
	}
	
	/*
	*	Class Constructor
	*	This gets called when the class is initalized.
	*/
	CrystalRender::CrystalRender(){
		this->InitVita2d();
	}
	
	/*
	*	Method to load a texture
	*/
	CrystalTexture CrystalRender::LoadImage(string filename){
		return vita2d_load_PNG_file(filename.c_str());
	}
	
	/*
	*	Set Draw Color
	*	Used for drawing text and other stuff
	*/
	void CrystalRender::SetDrawColor(int r, int g, int b, int a){
		this->DrawColor = RGBA8(r, g, b, a);
	}
	
	/*
	*	Set Draw Font
	* 	Used when drawing text
	*/
	void CrystalRender::SetDrawFont(CrystalDrawFont font){
		this->DrawFont = font;
	}
	
	/*
	*	Set Draw Size
	*	Used when drawing text
	*/
	void CrystalRender::SetDrawSize(CrystalDrawSize sz){
		this->DrawSize = sz;
	}
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																				   //
	//																																				   //
	//																																				   //
	//	Drawing methods																																   //
	//																																				   //
	//																																				   //
	//																																				   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/*
	*	Clear screen &
	*	Begin Drawing
	*/
	bool CrystalRender::DrawStart(){
		vita2d_start_drawing();
		vita2d_clear_screen();
		return true;
	}
	
	/*
	*	Draw text to the screen
	*/
	bool CrystalRender::DrawText(string text, int x, int y){
		if(this->DrawFont == CrystalPgfFont)
			vita2d_pgf_draw_text(this->Pgf, x, y, this->DrawColor, this->DrawSize, text.c_str());
		else if(this->DrawFont == CrystalPvfFont)
			vita2d_pvf_draw_text(this->Pvf, x, y, this->DrawColor, this->DrawSize, text.c_str());
		else
			return false;
		return true;
	}
	
	/*
	*	Draw image to the screen
	*/
	bool CrystalRender::DrawImage(CrystalTexture image, int x, int y){
		vita2d_draw_texture(image, (float)x, (float)y);
		return true;
	}
	
	/*
	*	Draw a rectangle to the screen
	*/
	
	bool CrystalRender::DrawRectangle(int x, int y, int w, int h)
	{
		vita2d_draw_rectangle((float)x, (float)y, (float)w, (float)h, this->DrawColor);
		return true;
	}
	
	/*
	*	End Drawing &
	*	Swap buffers
	*/
	bool CrystalRender::DrawEnd(){
		vita2d_end_drawing();
		vita2d_swap_buffers();
		return true;
	}
}

