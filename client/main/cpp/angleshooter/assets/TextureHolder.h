#pragma once

class TextureHolder final : public ResourceHolder<sf::Texture> {
	std::unique_ptr<sf::Texture> createDefaultTexture();

protected:
	explicit TextureHolder();
	~TextureHolder() = default;

public:
	inline static auto FONT_DOT = new Identifier("menu/font/dot.png");
	inline static auto FONT_0 = new Identifier("menu/font/0.png");
	inline static auto FONT_1 = new Identifier("menu/font/1.png");
	inline static auto FONT_2 = new Identifier("menu/font/2.png");
	inline static auto FONT_3 = new Identifier("menu/font/3.png");
	inline static auto FONT_4 = new Identifier("menu/font/4.png");
	inline static auto FONT_5 = new Identifier("menu/font/5.png");
	inline static auto FONT_6 = new Identifier("menu/font/6.png");
	inline static auto FONT_7 = new Identifier("menu/font/7.png");
	inline static auto FONT_8 = new Identifier("menu/font/8.png");
	inline static auto FONT_9 = new Identifier("menu/font/9.png");
	inline static auto FONT_A = new Identifier("menu/font/A.png");
	inline static auto FONT_a = new Identifier("menu/font/a_lowercase.png");
	inline static auto FONT_B = new Identifier("menu/font/B.png");
	inline static auto FONT_b = new Identifier("menu/font/b_lowercase.png");
	inline static auto FONT_C = new Identifier("menu/font/C.png");
	inline static auto FONT_c = new Identifier("menu/font/c_lowercase.png");
	inline static auto FONT_D = new Identifier("menu/font/D.png");
	inline static auto FONT_d = new Identifier("menu/font/d_lowercase.png");
	inline static auto FONT_E = new Identifier("menu/font/E.png");
	inline static auto FONT_e = new Identifier("menu/font/e_lowercase.png");
	inline static auto FONT_F = new Identifier("menu/font/F.png");
	inline static auto FONT_f = new Identifier("menu/font/f_lowercase.png");
	inline static auto FONT_G = new Identifier("menu/font/G.png");
	inline static auto FONT_g = new Identifier("menu/font/g_lowercase.png");
	inline static auto FONT_H = new Identifier("menu/font/H.png");
	inline static auto FONT_h = new Identifier("menu/font/h_lowercase.png");
	inline static auto FONT_I = new Identifier("menu/font/I.png");
	inline static auto FONT_i = new Identifier("menu/font/i_lowercase.png");
	inline static auto FONT_J = new Identifier("menu/font/J.png");
	inline static auto FONT_j = new Identifier("menu/font/j_lowercase.png");
	inline static auto FONT_K = new Identifier("menu/font/K.png");
	inline static auto FONT_k = new Identifier("menu/font/k_lowercase.png");
	inline static auto FONT_L = new Identifier("menu/font/L.png");
	inline static auto FONT_l = new Identifier("menu/font/l_lowercase.png");
	inline static auto FONT_M = new Identifier("menu/font/M.png");
	inline static auto FONT_m = new Identifier("menu/font/m_lowercase.png");
	inline static auto FONT_N = new Identifier("menu/font/N.png");
	inline static auto FONT_n = new Identifier("menu/font/n_lowercase.png");
	inline static auto FONT_O = new Identifier("menu/font/O.png");
	inline static auto FONT_o = new Identifier("menu/font/o_lowercase.png");
	inline static auto FONT_P = new Identifier("menu/font/P.png");
	inline static auto FONT_p = new Identifier("menu/font/p_lowercase.png");
	inline static auto FONT_Q = new Identifier("menu/font/Q.png");
	inline static auto FONT_q = new Identifier("menu/font/q_lowercase.png");
	inline static auto FONT_R = new Identifier("menu/font/R.png");
	inline static auto FONT_r = new Identifier("menu/font/r_lowercase.png");
	inline static auto FONT_S = new Identifier("menu/font/S.png");
	inline static auto FONT_s = new Identifier("menu/font/s_lowercase.png");
	inline static auto FONT_T = new Identifier("menu/font/T.png");
	inline static auto FONT_t = new Identifier("menu/font/t_lowercase.png");
	inline static auto FONT_U = new Identifier("menu/font/U.png");
	inline static auto FONT_u = new Identifier("menu/font/u_lowercase.png");
	inline static auto FONT_V = new Identifier("menu/font/V.png");
	inline static auto FONT_v = new Identifier("menu/font/v_lowercase.png");
	inline static auto FONT_W = new Identifier("menu/font/W.png");
	inline static auto FONT_w = new Identifier("menu/font/w_lowercase.png");
	inline static auto FONT_X = new Identifier("menu/font/X.png");
	inline static auto FONT_x = new Identifier("menu/font/x_lowercase.png");
	inline static auto FONT_Y = new Identifier("menu/font/Y.png");
	inline static auto FONT_y = new Identifier("menu/font/y_lowercase.png");
	inline static auto FONT_Z = new Identifier("menu/font/Z.png");
	inline static auto FONT_z = new Identifier("menu/font/z_lowercase.png");
	inline static auto NUMBERS = std::array{
		FONT_0, FONT_1, FONT_2, FONT_3, FONT_4, FONT_5, FONT_6, FONT_7, FONT_8, FONT_9, FONT_DOT
	};
	inline static auto ALPHABET = std::array{
		FONT_A, FONT_B, FONT_C, FONT_D, FONT_E, FONT_F, FONT_G, FONT_H, FONT_I,
		FONT_J, FONT_K, FONT_L, FONT_M, FONT_N, FONT_O, FONT_P, FONT_Q, FONT_R,
		FONT_S, FONT_T, FONT_U, FONT_V, FONT_W, FONT_X, FONT_Y, FONT_Z
	};
	inline static auto ALPHABET_LOWERCASE = std::array{
		FONT_a, FONT_b, FONT_c, FONT_d, FONT_e, FONT_f, FONT_g, FONT_h, FONT_i,
		FONT_j, FONT_k, FONT_l, FONT_m, FONT_n, FONT_o, FONT_p, FONT_q, FONT_r,
		FONT_s, FONT_t, FONT_u, FONT_v, FONT_w, FONT_x, FONT_y, FONT_z
	};
	inline static std::map<char, Identifier*> CHARACTER_MAP = {
		{'0', FONT_0}, {'1', FONT_1}, {'2', FONT_2}, {'3', FONT_3}, {'4', FONT_4},
		{'5', FONT_5}, {'6', FONT_6}, {'7', FONT_7}, {'8', FONT_8}, {'9', FONT_9},
		{'A', FONT_A}, {'B', FONT_B}, {'C', FONT_C}, {'D', FONT_D}, {'E', FONT_E},
		{'F', FONT_F}, {'G', FONT_G}, {'H', FONT_H}, {'I', FONT_I}, {'J', FONT_J},
		{'K', FONT_K}, {'L', FONT_L}, {'M', FONT_M}, {'N', FONT_N}, {'O', FONT_O},
		{'P', FONT_P}, {'Q', FONT_Q}, {'R', FONT_R}, {'S', FONT_S}, {'T', FONT_T},
		{'U', FONT_U}, {'V', FONT_V}, {'W', FONT_W}, {'X', FONT_X}, {'Y', FONT_Y},
		{'Z', FONT_Z},
		{'a', FONT_a}, {'b', FONT_b}, {'c', FONT_c}, {'d', FONT_d}, {'e', FONT_e},
		{'f', FONT_f}, {'g', FONT_g}, {'h', FONT_h}, {'i', FONT_i}, {'j', FONT_j},
		{'k', FONT_k}, {'l', FONT_l}, {'m', FONT_m}, {'n', FONT_n}, {'o', FONT_o},
		{'p', FONT_p}, {'q', FONT_q}, {'r', FONT_r}, {'s', FONT_s}, {'t', FONT_t},
		{'u', FONT_u}, {'v', FONT_v}, {'w', FONT_w}, {'x', FONT_x}, {'y', FONT_y},
		{'z', FONT_z},
		{'.', FONT_DOT}
	};
	sf::Texture getText(const std::string& string, int characterWidth = 28);

	explicit TextureHolder(const FontHolder&) = delete;
	void operator=(const TextureHolder&) = delete;
	static TextureHolder& getInstance() {
		static TextureHolder instance;
		return instance;
	}
};