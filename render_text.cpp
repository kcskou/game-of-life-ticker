#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace std;

void handle_error( string message, int error ) {
    cout << message << ": " << error << "\n";
    exit( 1 );
}

void draw_bitmap( unsigned char* buffer,
                  unsigned int width,
                  unsigned int height )
{
    unsigned int i, j;
    for ( i = 0; i < height; i++ ) {
        for ( j = 0; j < width; j++) {
            putchar( buffer[( i * width ) + j] == 0 ? '.' : '#' );
        }
        putchar( '\n' );
    }
}

void setup( FT_Library* lib,
            FT_Face* face, 
            FT_Error* error )
{
    *error = FT_Init_FreeType( lib );
    if ( *error ) handle_error( "library init error", *error );
    
    *error = FT_New_Face( *lib,
                         "/usr/share/fonts/truetype/msttcorefonts/arial.ttf",
                         0,
                         face );
    if ( *error ) handle_error( "new face error", *error );
    
    *error = FT_Set_Char_Size( *face,
                              24 * 64,
                              0,
                              72,
                              0 );
    if ( *error ) handle_error( "set char size error", *error );
}

void render_glyph( char* ch, 
                   FT_Face* face,
                   FT_GlyphSlot slot, 
                   FT_UInt* glyph_index,
                   FT_Error* error )
{
    *glyph_index = FT_Get_Char_Index( *face, *ch );
    *error = FT_Load_Glyph( *face,
                            *glyph_index,
                            FT_LOAD_RENDER |
                            FT_LOAD_TARGET_MONO );
    if ( *error ) handle_error( "load glyph error", *error );
}

void bit_blit( unsigned char* src,
               unsigned int src_width,
               unsigned int src_height,
               unsigned char* dst,
               unsigned int dst_width,
               int x,
               int y )
{
    unsigned int i, j;
    int src_index = 0;
    int row_offset = dst_width - src_width;
    int dst_index = y * dst_width + x;
    for ( i = 0; i < src_height; i++ )
    {
        for ( j = 0; j < src_width; j++ )
        {
            dst[dst_index++] |= src[src_index++];
        }
        dst_index += row_offset;
    }
}

FT_Pos kerning_offset( FT_Face* face, char prev, char curr )
{
    if ( prev == 0 ) {
        return 0;
    } else {
        FT_UInt prev_glyph_index = FT_Get_Char_Index( *face, prev );
        FT_UInt curr_glyph_index = FT_Get_Char_Index( *face, curr );
        FT_Vector* kerning = ( FT_Vector* ) malloc( sizeof( FT_Vector ) );
        FT_Get_Kerning( *face, prev_glyph_index, curr_glyph_index, FT_KERNING_UNFITTED, kerning );
        FT_Pos kerning_x = kerning->x / 64;
        free( kerning );
        return kerning_x;
    }
}

void unpack_mono_bitmap( FT_Bitmap src, unsigned char* dst_buffer )
{
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
}

void render( char* text )
{
    FT_Library lib;
    FT_Face face;
    FT_Error error;
    FT_GlyphSlot slot;
    FT_UInt glyph_index;
    FT_Pos asc, des, adv, glyph_width, kerning_x; 

    int n, num_chars, x, y;
    long max_ascent, max_descent;
    int target_width, target_height;
    char prev_char;
    unsigned char* unpacked_bitmap;
    unsigned char* image;
    
    setup( &lib, &face, &error );

    slot = face->glyph;
    num_chars = strlen( text );
    target_width = 0;
    max_ascent = 0;
    max_descent = 0;
    prev_char = 0;

    for ( n = 0; n < num_chars; n++ )
    {
        render_glyph( &text[n], &face, slot, &glyph_index, &error );
       
        asc = slot->metrics.horiBearingY / 64;
        des = ( slot->metrics.height / 64 ) - asc;
        adv = slot->advance.x / 64;
        glyph_width = slot->metrics.width / 64;

        max_ascent = std::max( asc, max_ascent );
        max_descent = std::max( des, max_descent );
        kerning_x = kerning_offset( &face, prev_char, text[n] );
        target_width += std::max( adv + kerning_x, glyph_width );
        prev_char = text[n];
    }
    
    target_height = max_ascent + max_descent;
    image = ( unsigned char* ) calloc( target_height * target_width, sizeof *image );

    x = 0;
    prev_char = 0;
    for ( n = 0; n < num_chars; n++ )
    {
        render_glyph( &text[n], &face, slot, &glyph_index, &error );
        kerning_x = kerning_offset( &face, prev_char, text[n] );
        x += kerning_x;
        y = target_height - max_descent - ( slot->metrics.horiBearingY / 64 );
        
        unpacked_bitmap = ( unsigned char * ) calloc( slot->bitmap.width * slot->bitmap.rows, sizeof *unpacked_bitmap );
        unpack_mono_bitmap( slot->bitmap, unpacked_bitmap );
        bit_blit( unpacked_bitmap,
                  slot->bitmap.width,
                  slot->bitmap.rows,
                  image,
                  target_width,
                  x, y );
        free( unpacked_bitmap );
        x += ( slot->advance.x / 64 );
        prev_char = text[n];
    }

    draw_bitmap( image, target_width, target_height );
    free( image );
    FT_Done_Face( face );
    FT_Done_FreeType( lib );
}

int main( int argc, char** argv )
{
    if ( argc != 2 )
    {
        fprintf( stderr, "usage: %s text-to-render\n", argv[0] );
        exit( 1 );
    }
    render( argv[1] );
    return 0;
}
