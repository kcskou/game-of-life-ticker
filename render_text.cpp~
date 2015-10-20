#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace std;

struct Dimension
{
    int width, height, baseline;
};

class Bitmap
{
    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned char *m_pixels;

    public:
        Bitmap( unsigned int, unsigned int );
        Bitmap( unsigned int, unsigned int, unsigned char * );
        ~Bitmap();
        void bit_blit( Bitmap *, int, int );
        void print();
};

Bitmap::Bitmap( unsigned int width, unsigned int height )
    : m_width( width ), m_height( height )
{
    m_pixels = ( unsigned char * ) calloc( width * height, sizeof *m_pixels );
} 

Bitmap::Bitmap( unsigned int width, unsigned int height,
                unsigned char *pixels ) 
    : m_width( width ), m_height( height ), m_pixels( pixels )
{ }

Bitmap::~Bitmap() { delete m_pixels; }

void Bitmap::bit_blit( Bitmap *src, int x, int y )
{
    unsigned int i, j;
    int src_index = 0;
    int row_offset = m_width - src->m_width;
    int dst_index = y * m_width + x;
    for ( i = 0; i < src->m_height; i++ )
    {
        for ( j = 0; j < src->m_width; j++ )
        {
            m_pixels[dst_index++] |= src->m_pixels[src_index++];
        }
        dst_index += row_offset;
    }
}

void Bitmap::print()
{
    unsigned int i, j;
    for ( i = 0; i < m_height; i++ ) {
        for ( j = 0; j < m_width; j++) {
            putchar( m_pixels[( i * m_width ) + j] == 0 ? '.' : '#' );
        }
        putchar( '\n' );
    }
}

class Glyph
{
    private:
        Bitmap* bitmap;
        int ascent;
        int descent;
        int advance;
        unsigned int width;
        unsigned int height;

        Bitmap* unpack_mono_bitmap( FT_Bitmap );

    public:
        Glyph( FT_GlyphSlot );
        ~Glyph();
        int get_width();
        int get_height();
        int get_ascent();
        int get_descent();
        int get_advance();
        Bitmap* get_bitmap();
};

Glyph::Glyph( FT_GlyphSlot slot )
{
        bitmap = unpack_mono_bitmap( slot->bitmap );
        width = slot->bitmap.width;
        height = slot->bitmap.rows;
        advance = (int) slot->advance.x / 64;
        ascent = max(0, (int) slot->metrics.horiBearingY / 64);
        descent = max(0, (int) ( slot->metrics.height / 64 ) - ascent);
}

Glyph::~Glyph()
{
    delete bitmap;
}

int Glyph::get_width() { return width; }
int Glyph::get_height() { return height; }
int Glyph::get_ascent() { return ascent; }
int Glyph::get_descent() { return descent; }
int Glyph::get_advance() { return advance; }
Bitmap* Glyph::get_bitmap() { return bitmap; }

Bitmap* Glyph::unpack_mono_bitmap( FT_Bitmap src )
{
    unsigned char* dst_buffer = 
        ( unsigned char* ) calloc( src.width * src.rows, sizeof *dst_buffer );
    int i, j, k;
    int num_bits_done, dst_start_index, bits_to_unpack;
    char byte, bit;
    for ( i = 0; i < ( int ) src.rows; i++ )
    {
        for ( j = 0; j < src.pitch; j++ )
        {
            byte = src.buffer[i * src.pitch + j];
            num_bits_done = j * 8;
            dst_start_index = i * src.width + num_bits_done;
            bits_to_unpack = std::min( ( int ) src.width - num_bits_done, 8 );
            for ( k = 0; k < bits_to_unpack; k++ )
            {
                bit = byte & (1 << (7 - k));
                dst_buffer[dst_start_index + k] = bit ? 1 : 0;
            }
        }
    }
    Bitmap *bitmap = new Bitmap( src.width, src.rows, dst_buffer );
    return bitmap;
}

class Font
{
    private:
        FT_Library lib;
        FT_Face face;
        FT_Error error;
        FT_UInt glyph_index;

        void handle_error( string, int );
        Glyph* render_glyph( char );
        Dimension get_dimension( string );
        int kerning_offset( char, char );

    public:
        Font( string, int );
        ~Font();
        Bitmap* render( string );
};

Font::Font( string filename, int size )
{
    error = FT_Init_FreeType( &lib );
    if ( error ) handle_error( "library init error", error );
    
    error = FT_New_Face( lib, filename.c_str(), 0, &face );
    if ( error ) handle_error( "new face error", error );
    
    error = FT_Set_Char_Size( face, size * 64, 0, 72, 0 );
    if ( error ) handle_error( "set char size error", error );
}

Font::~Font()
{
    FT_Done_Face( face );
    FT_Done_FreeType( lib );
}

Glyph* Font::render_glyph( char ch ) 
{
    glyph_index = FT_Get_Char_Index( face, ch );
    error = FT_Load_Glyph( face,
                           glyph_index,
                           FT_LOAD_RENDER |
                           FT_LOAD_TARGET_MONO );
    if ( error ) handle_error( "load glyph error", error );
    Glyph *glyph = new Glyph( face->glyph );
    return glyph;
}



Dimension Font::get_dimension( string text )
{
    int kerning_x;
    int target_width = 0;
    int max_ascent = 0;
    int max_descent = 0;
    char prev_char = 0;

    Glyph *glyph;
    Dimension dim;
    
    for ( char ch : text )
    {
        glyph = render_glyph( ch );
        max_ascent = max( glyph->get_ascent(), max_ascent );
        max_descent = max( glyph->get_descent(), max_descent );
        kerning_x = kerning_offset( prev_char, ch );
        target_width += 
            max( glyph->get_advance() + kerning_x, glyph->get_width() );
        prev_char = ch;
        delete glyph;
    }
    dim.width = target_width;
    dim.height = max_ascent + max_descent;
    dim.baseline = max_descent;
    return dim;

}

Bitmap* Font::render( string text )
{
    Dimension dim = get_dimension( text );
    
    int y;
    int x = 0;
    char prev_char = 0;
    Bitmap *bitmap = new Bitmap( dim.width, dim.height );
    Glyph* glyph;
    
    for ( char ch : text )
    {
        glyph = render_glyph( ch );
        x += kerning_offset( prev_char, ch );
        y = dim.height - dim.baseline - glyph->get_ascent();
        bitmap->bit_blit( glyph->get_bitmap(), x, y );
        x += glyph->get_advance();
        prev_char = ch;
    }
    return bitmap;

}


void Font::handle_error( string message, int error ) {
    cout << message << ": " << error << '\n';
    exit( 1 );
}



int Font::kerning_offset( char prev, char curr )
{
    if ( prev == 0 ) {
        return 0;
    } else {
        FT_UInt prev_glyph_index = FT_Get_Char_Index( face, prev );
        FT_UInt curr_glyph_index = FT_Get_Char_Index( face, curr );
        FT_Vector* kerning = new FT_Vector();
        FT_Get_Kerning( face, prev_glyph_index, curr_glyph_index,
                        FT_KERNING_UNFITTED, kerning );
        int kerning_x = kerning->x / 64;
        delete kerning;
        return kerning_x;
    }
}

int main( int argc, char** argv )
{
    if ( argc != 2 )
    {
        cerr << "usage: " << argv[0] << " text-to-render\n";
        exit( 1 );
    }

    Font *font = new Font( "arial.ttf", 24 );
    Bitmap *bitmap = font->render( argv[1] );
    bitmap->print(); // FIXME override operator << to print to cout
    delete bitmap;
    delete font;
    return 0;
}
