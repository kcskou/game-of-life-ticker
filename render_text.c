#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ft2build.h>
#include FT_FREETYPE_H

int max( int a, int b ) {
    return ( a > b ) ? a : b;
}

void handle_error( char* message, int error ) {
    printf( "%s: %d\n", message, error );
    exit( 1 );
}

void draw_bitmap( unsigned char* buffer,
                  unsigned int width,
                  unsigned int height )
{
    int i, j;
    for ( i = 0; i < height; i++ ) {
        for ( j = 0; j < width; j++) {
            putchar( buffer[( i * width ) + j] == 0 ? ' ' : '#' );
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
                              100 * 64,
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
                            FT_LOAD_TARGET_MONO );
    if ( *error ) handle_error( "load glyph error", *error );
        
    *error = FT_Render_Glyph( slot, FT_RENDER_MODE_MONO );
    if ( *error ) handle_error( "render glyph error", *error );
        
    /* draw_bitmap( slot->bitmap.buffer,
                 slot->bitmap.pitch,
                 slot->bitmap.rows );
    */
    printf( "%c\n", *ch );
}

void bit_blit( unsigned char* src,
               unsigned int src_width,
               unsigned int src_height,
               unsigned char* dst,
               int x,
               int y )
{
    // FIXME: copy glyph to image starting at position x, y 
}

void render( char* text )
{
    FT_Library lib;
    FT_Face face;
    FT_Error error;
    FT_GlyphSlot slot;
    FT_UInt glyph_index;
    FT_Pos asc, des, adv; 

    int n, num_chars, x, y;
    int max_ascent, max_descent;
    int target_width, target_height, target_baseline;
    unsigned char* image;
    
    setup( &lib, &face, &error );

    slot = face->glyph;
    num_chars = strlen( text );
    target_width = 0;
    max_ascent = 0;
    max_descent = 0;

    for ( n = 0; n < num_chars; n++ )
    {
        render_glyph( &text[n], &face, slot, &glyph_index, &error );
       
        asc = slot->metrics.horiBearingY / 64;
        des = ( slot->metrics.height / 64 ) - asc;
        adv = slot->metrics.horiAdvance / 64;
        printf( "Ascent: %ld; Descent: %ld; Advance: %ld\n", asc, des, adv );

        target_width += adv;
        max_ascent = max( asc, max_ascent );
        max_descent = max( des, max_descent );
    }
    
    target_height = max_ascent + max_descent;
    printf( "Bitmap metrics -- width: %d; height: %d; baseline: %d\n",
            target_width,
            target_height,
            max_descent );

    image = ( char* ) calloc( target_height * target_width, sizeof( char ) );

    x = 0;
    for ( n = 0; n < num_chars; n++ )
    {
        render_glyph( &text[n], &face, slot, &glyph_index, &error );
        y = target_height - max_descent - ( slot->metrics.horiBearingY / 64 );
        bit_blit( slot->bitmap.buffer,
                  slot->bitmap.pitch,
                  slot->bitmap.rows,
                  image, x, y );
        x += slot->metrics.horiAdvance / 64;
    } 
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
