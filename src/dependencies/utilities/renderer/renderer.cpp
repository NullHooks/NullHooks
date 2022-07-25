#include "dependencies/utilities/renderer/renderer.hpp"

void render::initialize() {
	render::fonts::watermark_font = interfaces::surface->font_create();
	render::fonts::watermark_font_m = interfaces::surface->font_create();
	render::fonts::watermark_font_ns = interfaces::surface->font_create();
	render::fonts::playername_font = interfaces::surface->font_create();
	render::fonts::weapon_icon_font = interfaces::surface->font_create();
	render::fonts::dina_font = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph(render::fonts::watermark_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::watermark_font_m, "Tahoma", 14, 500, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::watermark_font_ns, "Tahoma", 12, 500, 0, 0, 0);
	interfaces::surface->set_font_glyph(render::fonts::playername_font, "Lucida Console", 12, 500, 0, 0, 0);

	// Other
	interfaces::surface->set_font_glyph(render::fonts::weapon_icon_font, "csgo_icons", 30, 300, 0, 0, 0x210);			// No worky (it broky)
	interfaces::surface->set_font_glyph(render::fonts::playername_font, "Dina", 12, 500, 0, 0, 0);						// A personal favourite https://www.dcmembers.com/jibsen/download/61/
}

void render::draw_line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void render::draw_text_string(std::int32_t x, std::int32_t y, unsigned long font, std::string string, bool text_centered, color colour) {
	const auto converted_text = std::wstring(string.begin(), string.end());

	int width, height;
	interfaces::surface->get_text_size(font, converted_text.c_str(), width, height);

	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	
	if (text_centered) interfaces::surface->draw_text_pos(x - (width / 2), y);
	else interfaces::surface->draw_text_pos(x, y);
	
	interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
}

void render::draw_text_wchar(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t* string, bool text_centered, color colour) {
	int width, height;
	interfaces::surface->get_text_size(font, string, width, height);

	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	
	if (text_centered) interfaces::surface->draw_text_pos(x - (width / 2), y);
	else interfaces::surface->draw_text_pos(x, y);

	interfaces::surface->draw_render_text(string, wcslen(string));
}

void render::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_filled_rectangle(x, y, width, height);
}

void render::draw_outline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_textured_polygon(std::int32_t vertices_count, vertex_t* vertices, color color) {
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	unsigned int texture_id{};
	if (!texture_id) {
		texture_id = interfaces::surface->create_new_texture_id(true);
		interfaces::surface->set_texture_rgba(texture_id, buf, 1, 1);
	}
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_polygon(vertices_count, vertices);
}

void render::draw_circle(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color) {
	float step = M_PI * 2.0 / segments;
	for (float a = 0; a < (M_PI * 2.0); a += step) {
		float x1 = radius * cos(a) + x;
		float y1 = radius * sin(a) + y;
		float x2 = radius * cos(a + step) + x;
		float y2 = radius * sin(a + step) + y;
		interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
}

vec2_t render::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}

void render::draw_fade(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color1, color color2, bool horizontal) {
	interfaces::surface->set_drawing_color(color1.r, color1.g, color1.b, color1.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, 255, 0, horizontal);

	interfaces::surface->set_drawing_color(color2.r, color2.g, color2.b, color2.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, 0, 255, horizontal);
}