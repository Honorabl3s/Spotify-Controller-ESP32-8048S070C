SET(SOURCES ui_ImgCover.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_bg_png.c
    ui_img_play_pause_png.c
    ui_img_coveralbum1_png.c
    ui_img_104607339.c
    ui_img_next_png.c
    ui_img_prev_png.c
    fonts/ui_font_FontArtis24.c
    fonts/ui_font_FontJudul48.c
    fonts/ui_font_FontJepang24.c
    fonts/ui_font_FontJepang48.c)

add_library(ui ${SOURCES})
