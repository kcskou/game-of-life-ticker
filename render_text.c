#include <stdio.h>
#include <string.h>
#include <ft2build.h>
#include FT_FREETYPE_H

void handle_error( char* message, int error ) {
    printf( "%s: %d\n", message, error );
    exit( 1 );
}

void draw_bitmap( FT_Bitmap* bitmap )
{
    int i, j;
    for ( i = 0; i < bitmap->rows; i++ ) {
        for ( j = 0; j < bitmap->pitch; j++) {
            putchar( bitmap->buffer[( i*bitmap->pitch ) + j] == 0 ? ' ' : '#' );
        }
        putchar( '\n' );
    }
}

void print_metrics( FT_Glyph_Metrics* metrics ) {
    FT_Pos asc = metrics->horiBearingY / 64;
    FT_Pos des = ( metrics->height / 64 ) - asc;
    FT_Pos adv = metrics->horiAdvance / 64;
    printf("Ascent: %ld; Descent: %ld; Advance: %ld\n", asc, des, adv);
}

void render( char* text )
{
    FT_Library lib;
    FT_Face face;
    FT_Error error;
    FT_GlyphSlot slot;
    FT_UInt glyph_index;

    int n, num_chars;

    error = FT_Init_FreeType( &lib );
    if ( error ) handle_error( "library init error", error );
    
    error = FT_New_Face( lib,
                         "/usr/share/fonts/truetype/msttcorefonts/arial.ttf",
                         0,
                         &face );
    if ( error ) handle_error( "new face error", error );
    
    error = FT_Set_Char_Size( face,
                              100 * 64,
                              0,
                              72,
                              0 );
    if ( error ) handle_error( "set char size error", error );
    
    slot = face->glyph;
    num_chars = strlen( text );
    
    for ( n = 0; n < num_chars; n++ )
    {
        glyph_index = FT_Get_Char_Index( face, text[n] );    
        error = FT_Load_Glyph( face,
                               glyph_index,
                               FT_LOAD_TARGET_MONO );
        if ( error ) handle_error( "load glyph error", error );
        
        error = FT_Render_Glyph( slot, FT_RENDER_MODE_MONO );
        if ( error ) handle_error( "render glyph error", error );
        
        draw_bitmap( &slot->bitmap );
        print_metrics( &slot->metrics );
    }

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
