#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H

void draw_bitmap( FT_Bitmap* bitmap )
{
    int i, j;
    for ( i = 0; i < bitmap->rows; i++ ) {
        for ( j = 0; j < bitmap->width; j++) {
            putchar( bitmap->buffer[(i*bitmap->width) + j] == 0 ? ' ' : '*' );
        }
        putchar( '\n' );
    }
}

int main( int argc, char** argv )
{
    FT_Library lib;
    FT_Face face;
    FT_Error error;
    FT_GlyphSlot slot;
    FT_UInt glyph_index;
    
    error = FT_Init_FreeType( &lib );
    if ( error ) 
    {
        printf( "library initialization error: %d\n", error );
    }
    error = FT_New_Face( lib,
                         "/usr/share/fonts/truetype/msttcorefonts/arial.ttf",
                         0,
                         &face );
    if ( error )
    {
       printf( "FT_New_Face error: %d\n", error );
    } 

    error = FT_Set_Char_Size( face,
                              16 * 64,
                              0,
                              500,
                              0 );
    
    slot = face->glyph;

    glyph_index = FT_Get_Char_Index( face, 'K' );    
    error = FT_Load_Glyph( face,
                           glyph_index,
                           FT_LOAD_MONOCHROME );
    if ( error )
    {
        printf( "Load Glyph error: %d\n", error );
    }

    error = FT_Render_Glyph( slot, FT_RENDER_MODE_MONO );
    if ( error )
    {
        printf( "Render Glyph error: %d\n", error );
    }
    
    draw_bitmap( &slot->bitmap );


    printf( "%s\n", face->style_name );
    printf( "Running just fine.\n" );    
    return 0;
}
