#include "Canvas.h"
#include "Main.h"

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
	EVT_SIZE(Canvas::Resized)
	EVT_LEFT_DOWN(Canvas::OnMouseDown)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	mainPtr = (Main*)parent;
	srand(time(0));

	cube.ApplySequence(Sequence::RandomSequence(25));

	CrossSolver solver;
	Sequence cross = solver(cube);
	cube.ApplySequence(cross);
}

Canvas::~Canvas()
{

}

void Canvas::OnDraw(wxDC& dc)
{
	dc.Clear();

	//Drawing here
	float faceWidth = std::min((h * 0.9) / 3.0, (w * 0.9) / 4.0);
	DrawFace(dc, cube, (int)FaceEnum::UP, faceWidth, 0.0, faceWidth); // Up
	DrawFace(dc, cube, (int)FaceEnum::LEFT, 0.0, faceWidth, faceWidth); // Left
	DrawFace(dc, cube, (int)FaceEnum::FRONT, faceWidth, faceWidth, faceWidth); //Front
	DrawFace(dc, cube, (int)FaceEnum::RIGHT, faceWidth * 2, faceWidth, faceWidth); // Right
	DrawFace(dc, cube, (int)FaceEnum::BACK, faceWidth * 3, faceWidth, faceWidth); // Back
	DrawFace(dc, cube, (int)FaceEnum::DOWN, faceWidth, faceWidth * 2, faceWidth); // Down
}

void Canvas::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC pdc(this);
	wxGCDC dc(pdc);
	this->GetSize(&w, &h);
	this->PrepareDC(dc);
	this->OnDraw(dc);
	evt.Skip();
}

void Canvas::Resized(wxSizeEvent& evt)
{
	this->Refresh();
	evt.Skip();
}

void Canvas::OnMouseDown(wxMouseEvent& evt)
{
	Refresh();
	evt.Skip();
}