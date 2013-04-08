#include "TStyle.h"

TStyle *prdStyle;

//======================================================================

// prdGrid: Turns the grid lines on (true) or off (false)

void prdGrid(bool gridOn) {
  prdStyle->SetPadGridX(gridOn);
  prdStyle->SetPadGridY(gridOn);
}

// fixOverlay: Redraws the axis

void prdFixOverlay() {
  gPad->RedrawAxis();
}

void setPRDStyle() {
  prdStyle = new TStyle("prdStyle","My very own style");

// For the canvas:
  prdStyle->SetCanvasBorderMode(0);
  prdStyle->SetCanvasColor(kWhite);
  prdStyle->SetCanvasDefH(600); //Height of canvas
  prdStyle->SetCanvasDefW(600); //Width of canvas
  prdStyle->SetCanvasDefX(0);   //POsition on screen
  prdStyle->SetCanvasDefY(0);

// For the Pad:
  prdStyle->SetPadBorderMode(0);
  // prdStyle->SetPadBorderSize(Width_t size = 1);
  prdStyle->SetPadColor(kWhite);
  prdStyle->SetPadGridX(false);
  prdStyle->SetPadGridY(false);
  prdStyle->SetGridColor(0);
  prdStyle->SetGridStyle(3);
  prdStyle->SetGridWidth(1);

// For the frame:
  prdStyle->SetFrameBorderMode(0);
  prdStyle->SetFrameBorderSize(1);
  prdStyle->SetFrameFillColor(0);
  prdStyle->SetFrameFillStyle(0);
  prdStyle->SetFrameLineColor(1);
  prdStyle->SetFrameLineStyle(1);
  prdStyle->SetFrameLineWidth(1);

// For the histo:
  // prdStyle->SetHistFillColor(1);
  // prdStyle->SetHistFillStyle(0);
  prdStyle->SetHistLineColor(1);
  prdStyle->SetHistLineStyle(0);
  prdStyle->SetHistLineWidth(1);
  // prdStyle->SetLegoInnerR(Float_t rad = 0.5);
  // prdStyle->SetNumberContours(Int_t number = 20);

  prdStyle->SetEndErrorSize(2);
  //prdStyle->SetErrorMarker(20); // deprecated, apparently
  //prdStyle->SetErrorX(0.);

  //prdStyle->SetMarkerStyle(20);

//For the fit/function:
  prdStyle->SetOptFit(1);
  prdStyle->SetFitFormat("5.4g");
  prdStyle->SetFuncColor(2);
  prdStyle->SetFuncStyle(1);
  prdStyle->SetFuncWidth(1);

//For the date:
  prdStyle->SetOptDate(0);
  // prdStyle->SetDateX(Float_t x = 0.01);
  // prdStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  prdStyle->SetOptFile(0);
  prdStyle->SetOptStat("e"); // To display the mean and RMS:   SetOptStat("mr");
  prdStyle->SetStatColor(kWhite);
  prdStyle->SetStatFont(42);
  //prdStyle->SetStatFontSize(0.025);
  prdStyle->SetStatFontSize(0.5);
  prdStyle->SetStatTextColor(1);
  prdStyle->SetStatFormat("6.4g");
  prdStyle->SetStatBorderSize(1);
  prdStyle->SetStatH(0.1);
  prdStyle->SetStatW(0.15);
  // prdStyle->SetStatStyle(Style_t style = 1001);
  // prdStyle->SetStatX(Float_t x = 0);
  // prdStyle->SetStatY(Float_t y = 0);

// Margins:
  prdStyle->SetPadTopMargin(0.05);
  prdStyle->SetPadBottomMargin(0.13);
  //prdStyle->SetPadLeftMargin(0.13);
  prdStyle->SetPadRightMargin(0.05);
  prdStyle->SetPadLeftMargin(0.16);  // zrap setting
  //prdStyle->SetPadRightMargin(0.02); // zrap setting

// For the Global title:
  prdStyle->SetOptTitle(1);
  prdStyle->SetTitleFont(42);
  prdStyle->SetTitleColor(1);
  prdStyle->SetTitleTextColor(1);
  prdStyle->SetTitleFillColor(10);
  prdStyle->SetTitleFontSize(0.05);
  // prdStyle->SetTitleH(0); // Set the height of the title box
  // prdStyle->SetTitleW(0); // Set the width of the title box
  // prdStyle->SetTitleX(0); // Set the position of the title box
  // prdStyle->SetTitleY(0.985); // Set the position of the title box
  // prdStyle->SetTitleStyle(Style_t style = 1001);
  // prdStyle->SetTitleBorderSize(2);

// For the axis titles:
  prdStyle->SetTitleColor(1, "XYZ");
  prdStyle->SetTitleFont(42, "XYZ");
  prdStyle->SetTitleSize(0.05, "XYZ");
  // prdStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // prdStyle->SetTitleYSize(Float_t size = 0.02);
  prdStyle->SetTitleXOffset(0.9);
  prdStyle->SetTitleYOffset(1.05);
  //prdStyle->SetTitleYOffset(1.25);        // zrap setting
  // prdStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:
  prdStyle->SetLabelColor(1, "XYZ");
  prdStyle->SetLabelFont(42, "XYZ");
  prdStyle->SetLabelOffset(0.007, "XYZ");
  prdStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  prdStyle->SetAxisColor(1, "XYZ");
  prdStyle->SetStripDecimals(kTRUE);
  prdStyle->SetTickLength(0.03, "XYZ");
  prdStyle->SetNdivisions(510, "XYZ");
  prdStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  prdStyle->SetPadTickY(1);

//PRD addition: TLegend option:
  prdStyle->SetLegendBorderSize(1);

// Change for log plots:
  prdStyle->SetOptLogx(0);
  prdStyle->SetOptLogy(0);
  prdStyle->SetOptLogz(0);

// Postscript options:
  prdStyle->SetPaperSize(20.,20.);
  // prdStyle->SetLineScalePS(Float_t scale = 3);
  // prdStyle->SetLineStyleString(Int_t i, const char* text);
  // prdStyle->SetHeaderPS(const char* header);
  // prdStyle->SetTitlePS(const char* pstitle);

  // prdStyle->SetBarOffset(Float_t baroff = 0.5);
  // prdStyle->SetBarWidth(Float_t barwidth = 0.5);
  // prdStyle->SetPaintTextFormat(const char* format = "g");
  // prdStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // prdStyle->SetTimeOffset(Double_t toffset);
  // prdStyle->SetHistMinimumZero(kTRUE);

  prdStyle->SetPalette(1,0);

  prdStyle->cd();
}
