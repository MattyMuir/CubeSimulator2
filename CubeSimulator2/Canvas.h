#pragma once
#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>

#include <string>

#include "Cube.h"
#include "CrossSolver.h"
#include "Timer.h"

class Main;

class Canvas : wxPanel
{
public:
	Canvas(wxWindow* parent);
	~Canvas();
	Main* mainPtr;

protected:
	int w = 0, h = 0;
	Cube<3> cube;

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);

	template <size_t Order>
	void DrawFace(wxDC& dc, const Cube<Order>& cube, int faceIndex, float cx, float cy, float width)
	{
		wxColour cols[6] = { { 255, 255, 255}, { 255, 255, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, { 255, 0, 0 }, { 255, 150, 0 } };

		// (cx, cy) are top left of face
		float tileWidth = width / Order;

		wxBrush brush = dc.GetBrush();

		cy += width;
		for (int y = 0; y < Order; y++)
		{
			for (int x = 0; x < Order; x++)
			{
				brush.SetColour(cols[(int)cube.faces[faceIndex][y * Order + x].col]);
				dc.SetBrush(brush);

				float screenX = cx + x * tileWidth;
				float screenY = cy - (y + 1) * tileWidth;
				dc.GetGraphicsContext()->DrawRectangle(screenX, screenY, tileWidth, tileWidth);
			}
		}
	}

	void OnMouseDown(wxMouseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};