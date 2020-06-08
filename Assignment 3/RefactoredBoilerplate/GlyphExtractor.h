// ==========================================================================
// Font File Reading Support Code for CPSC 453
//  - requires the FreeType development libraries: http://www.freetype.org
//
// This module defines MySegment, MyContour, and MyGlyph data structures, as
// well as a GlyphExtractor class that will retrieve glyph outlines from a
// font file for specified characters. Data structures are as follows:
//  - A glyph consists of zero or more contours, plus an advance width
//  - A contour consists of one or more segments (stored as std::vector)
//  - A segment is either a straight line, quadratic Bezier, or cubic Bezier
//
// You may use this code (or not) however you see fit for your work.
//
// Author:  Sonny Chan
//          University of Calgary
// Date:    February 2016
// ==========================================================================
#ifndef GLYPHEXTRACTOR_H
#define GLYPHEXTRACTOR_H

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

// --------------------------------------------------------------------------
// DATA STRUCTURES: Segment, Contour, and Glyph

// A segment encodes a point, a straight line segment, a quadratic Bezier curve,
// or a cubic Bezier curve, as indicated by its degree field.
struct MySegment
{
    // degree of Bezier curve segment (0=point, 1=line, 2=quadratic, 3=cubic)
    unsigned int degree;

    // X and Y coordinates: only indices up to [degree] are valid
    float x[4], y[4];

    MySegment(int d = 0) : degree(d)
    {}
};

// An contour is a Bezier spline: a sequence of curve segments that share endpoints.
typedef std::vector<MySegment> MyContour;

// A glyph consists of a set of contours and an advance width to the next glyph.
struct MyGlyph
{
    // advance width to next glyph, in EM units
    float advance;

    // contours that form this glyph, in EM-box coordinates
    std::vector<MyContour> contours;

    MyGlyph(float adv = 0) : advance(adv)
    {}
};

// --------------------------------------------------------------------------
// This class encapsulates functionality required to load a font file from
// disk and retrieve glyph outlines for characters from the font.

class GlyphExtractor
{
    FT_Library  m_library;
    FT_Face     m_face;

    // private methods to print font/glyph info, for debugging
    void PrintFontInformation() const;
    void PrintGlyphInformation(int character) const;

public:
    GlyphExtractor();

    // call this method first to load a font file
    bool LoadFontFile(const std::string &filename);

    // this method retrieves a (possibly composite) glyph for the given character
    MyGlyph ExtractGlyph(int character) const;
};

// --------------------------------------------------------------------------
#endif // GLYPHEXTRACTOR_H
