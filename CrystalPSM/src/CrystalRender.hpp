/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_RENDERER_
#define _CRYSTAL_RENDERER_
#include <vita2d.h>
#include <vitasdk.h>

using namespace std;

namespace EliCrystal {
	typedef vita2d_texture* CrystalTexture;
	typedef uint32_t CrystalDrawColor;
	typedef float CrystalDrawSize;

	enum CrystalDrawFont{
		CrystalPgfFont,
		CrystalPvfFont
	};

	class CrystalRender{
		private:
			int TotalImages;
			vita2d_pgf *Pgf;
			vita2d_pvf *Pvf;
			CrystalDrawColor DrawColor;
			CrystalDrawFont DrawFont;
			CrystalDrawSize DrawSize;
			
			void InitVita2d();
		public:
			CrystalRender();
			CrystalTexture LoadImage(string filename);
			
			void SetDrawColor(int r, int g, int b, int a);
			void SetDrawFont(CrystalDrawFont font);
			void SetDrawSize(CrystalDrawSize sz);
			
			bool DrawStart();
			bool DrawText(string text, int x, int y);
			bool DrawRectangle(int x, int y, int w, int h);
			bool DrawImage(CrystalTexture image, int x, int y);
			bool DrawEnd();
	};
}


#endif
