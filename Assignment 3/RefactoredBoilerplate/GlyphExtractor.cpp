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

#include "GlyphExtractor.h"
#include <iostream>

// set this true to print information about the font loaded and glyphs extracted
#define DEBUG_PRINT 0

using namespace std;

// --------------------------------------------------------------------------

GlyphExtractor::GlyphExtractor()
    : m_face(0)
{
    // initialize freetype library
    FT_Error error = FT_Init_FreeType(&m_library);
    if (error) {
        cout << "ERROR: FreeType failed to initialize!" << endl;
    }
}

// --------------------------------------------------------------------------

bool GlyphExtractor::LoadFontFile(const string &filename)
{
    FT_Error error = FT_New_Face(m_library, filename.c_str(), 0, &m_face);

    if (error == FT_Err_Unknown_File_Format) {
        cout << "Freetype ERROR: unsupported file format in " << filename << endl;
        return false;
    }
    else if (error) {
        cout << "FreeType ERROR: unknown error occurred." << endl;
        return false;
    }

    if (DEBUG_PRINT) PrintFontInformation();

    return true;
}

// --------------------------------------------------------------------------

void GlyphExtractor::PrintFontInformation() const
{
    cout << "Font information for typeface " << m_face->family_name
         << " (" << m_face->style_name << "):" << endl;
    cout << "  Number of glyphs: \t" << m_face->num_glyphs << endl;
    cout << "  Units per EM: \t" << m_face->units_per_EM << endl;
}

void GlyphExtractor::PrintGlyphInformation(int character) const
{
    FT_Outline &outline = m_face->glyph->outline;

    cout << "Glyph information for character "
         << character << " (" << char(character) << "):" <<  endl;
    cout << "  Advance: " << m_face->glyph->advance.x
         << ", " << m_face->glyph->advance.y << endl;
    cout << "  Number of contours: " << outline.n_contours << endl;
    cout << "  Number of points:   " << outline.n_points << endl;

    cout << "  Points:";
    for (int i = 0; i < outline.n_points; ++i)
    {
        FT_Vector v = outline.points[i];
        cout << " (" << v.x << "," << v.y << ")";
        if (outline.tags[i] & 1) cout << "+";
        else cout << "-" << ((outline.tags[i] & 2) ? '3' : '2');
    }
    cout << endl;
}

// --------------------------------------------------------------------------

MyGlyph GlyphExtractor::ExtractGlyph(int character) const
{
    // first check that a font has been loaded
    if (!m_face) {
        cout << "GlyphExtractor ERROR: No font loaded!" << endl;
        return MyGlyph();
    }

    // look up the glyph index for the given character code
    int index = FT_Get_Char_Index(m_face, character);

    // load the glyph for the given character into the face glyph slot,
    // keeping the outline in original font units
    FT_Error error = FT_Load_Glyph(m_face, index, FT_LOAD_NO_SCALE);
    if (error || m_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    {
        cout << "FreeType ERROR: Could not find glyph outline for character "
             << character << " (" << char(character) << ")" <<  endl;
        return MyGlyph();
    }

    if (DEBUG_PRINT) PrintGlyphInformation(character);

    // create a new glyph structure to populate with this character outline
    FT_Outline &outline = m_face->glyph->outline;
    float em = m_face->units_per_EM;
    MyGlyph glyph(m_face->glyph->advance.x / em);

    // current point index
    int begin = 0;

    // iterate through the outline's contours
    for (int c = 0; c < outline.n_contours; ++c)
    {
        MyContour contour;

        // iterate through current contour's points
        int end = outline.contours[c];
        for (int p = begin; p <= end; ++p)
        {
            // index for next point, q
            int q = p+1;
            if (q > end) q = begin;

            // retrieve position vectors
            FT_Vector r_p = outline.points[p];
            FT_Vector r_q = outline.points[q];

            // create a segment to store control points
            MySegment segment;

            if (outline.tags[p] & 1) {
                segment.x[0] = r_p.x / em;
                segment.y[0] = r_p.y / em;
            }
            else {
                segment.x[0] = 0.5f * (r_p.x + r_q.x) / em;
                segment.y[0] = 0.5f * (r_p.y + r_q.y) / em;
            }

            // set degree of segment based on what the next point is
            if (outline.tags[q] & 1)
            {
                // next point is on curve, so this is a line segment
                segment.degree = 1;
                segment.x[1] = r_q.x / em;
                segment.y[1] = r_q.y / em;
            }
            else if (outline.tags[q] & 2)
            {
                // next point is third degree, so this is a cubic segment
                segment.degree = 3;
                for (int i = 0; i < 3; ++i)
                {
                    segment.x[1+i] = r_q.x / em;
                    segment.y[1+i] = r_q.y / em;
                    if (++q > end) q = begin;
                    r_q = outline.points[q];
                }
                p += 2;
            }
            else
            {
                // next point is second degree, so this is a quadratic segment
                segment.degree = 2;
                segment.x[1] = r_q.x / em;
                segment.y[1] = r_q.y / em;

                // advance q
                if (++q > end) q = begin;
                r_q = outline.points[q];

                // if the next point is on curve, store and advance p
                if (outline.tags[q] & 1) {
                    segment.x[2] = r_q.x / em;
                    segment.y[2] = r_q.y / em;
                    ++p;
                }
                // otherwise store the midpoint
                else {
                    segment.x[2] = 0.5f * (segment.x[1] + r_q.x / em);
                    segment.y[2] = 0.5f * (segment.y[1] + r_q.y / em);
                }
            }

            // add segment to contour
            contour.push_back(segment);
        }

        // set beginning of next contour
        begin = end + 1;

        // add contour to glyph
        glyph.contours.push_back(contour);
    }

    return glyph;
}

// --------------------------------------------------------------------------
