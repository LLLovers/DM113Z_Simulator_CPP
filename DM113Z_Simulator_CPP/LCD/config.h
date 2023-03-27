#pragma once

#define _COLOUR_BG	GetColor(128,210,0)		/*îwåiÇÃêF*/
#define _COLOUR_ON	GetColor(30,70,25)		/*ì_ìîéûÇÃêF*/
#define _COLOUR_OFF	GetColor(100,185,10)	/*è¡ìîéûÇÃêF*/


class config {
public:
	class offset {
	public:
		class bank_pgm {
		public:
			const static int x = 0;
			const static int y = -8;
		};

		class pan {
		public:
			const static int x = 0;
			const static int y = 8;
		};

		class arc {
		public:
			const static int x = 0;
			const static int y = 16;
		};
	};

	class window {
	public:
		const static int width = 1403;
		const static int height = 455;
		const static int margin_x = 32;
		const static int margin_y = 16;
	};

	class text {
	public:
		const static int row = 2;
		const static int column = 17;
		const static int pixel_width = 12;
		const static int pixel_height = 12;
		const static int pixel_gap = 1;
		const static int margin_x = 0;
		const static int margin_y = 0;
	};

	class ch {
	public:
		const static int pixel_width = config::text::pixel_width;
		const static int pixel_height = config::text::pixel_height;
		const static int pixel_gap = config::text::pixel_gap;
		const static int margin_x = 0;
		const static int margin_y = 8;
	};

	class bank_pgm {
	public:
		const static int pixel_width = config::text::pixel_width;
		const static int pixel_height = config::text::pixel_height;
		const static int pixel_gap = config::text::pixel_gap;
		const static int margin_x = 0;
		const static int margin_y = 4;
	};

	class line {
	public:
		const static int width = 64;
		const static int height = 32;
		const static int margin_x = 8;
		const static int margin_y = 8;
	};

	class mic {
	public:
		const static int width = config::line::width;
		const static int height = config::line::height;
		const static int margin_x = 4;
		const static int margin_y = 2;
	};

	class part {
	public:
		const static int pixel_width = 10;
		const static int pixel_height = 10;
		const static int pixel_gap = 1;
		const static int margin_x = 20;
		const static int margin_y = 0;
	};

	class midi {
	public:
		const static int pixel_width = config::part::pixel_width;
		const static int pixel_height = config::part::pixel_height;
		const static int pixel_gap = config::part::pixel_gap;
		const static int margin_x = 28;
		const static int margin_y = 0;
	};

	class gfx {
	public:
		const static int width = 16;
		const static int height = 16;
		const static int pixel_width = 12;
		const static int pixel_height = 6;
		const static int pixel_gap = 1;
		const static int margin_x = 40;
		const static int margin_y = 0;
	};

	class vol {
	public:
		const static int pixel_width = 52;
		const static int pixel_height = 7;
		const static int pixel_gap = 1;

		const static int margin_x = 32;
		const static int margin_y = 0;
	};

	class exp {
	public:
		const static int pixel_width = config::vol::pixel_width;
		const static int pixel_height = config::vol::pixel_height;
		const static int pixel_gap = config::vol::pixel_gap;

		const static int margin_x = 16;
		const static int margin_y = 0;
	};

	class pan {
	public:
		const static int margin_x = 12;
		const static int margin_y = 0;
	};

	class rev {
	public:
		const static int margin_x = 12;
		const static int margin_y = 0;
	};

	class cho {
	public:
		const static int margin_x = 12;
		const static int margin_y = 0;
	};

	class var {
	public:
		const static int margin_x = -4;
		const static int margin_y = 0;
	};

	class key {
	public:
		const static int margin_x = 12;
		const static int margin_y = 0;
	};

	class mode {
	public:
		const static int gap = 12;
		const static int margin_x = 0;
		const static int margin_y = 0;
	};

	class arrow {
	public:
		const static int margin_y = 2;
	};
};


/*bÇÕBIT MASKÇ≈Ç∑Éà*/

class segment_map {
public:
	class ch_left {
	public:
		const static int x = 3;
		const static int y = 2;
		const static int l = 3;
		const static int b = 1;
	};

	class ch_right {
	public:
		const static int x = 3;
		const static int y = 2;
		const static int l = 4;
		const static int b = 1;
	};

	class bank_pgm_left {
	public:
		const static int x = 3;
		const static int y = 2;
		const static int l = 0;
		const static int b = 1;
	};

	class bank_pgm_right {
	public:
		const static int x = 3;
		const static int y = 2;
		const static int l = 5;
		const static int b = 1;
	};

	class mic {
	public:
		const static int x = 3;
		const static int y = 2;
		const static int l = 1;
		const static int b = 1;
	};

	class line {
	public:
		const static int x = 3;
		const static int y = 2;
		const static int l = 2;
		const static int b = 1;
	};



	class vol {
	public:
		const static inline int x[] = { 19,19,19,19,19,19,19,19, };
		const static inline int y[] = { 0,0,0,0,0,0,0,0 };
		const static inline int l[] = { 0,1,2,3,4,5,6,7 };
		const static inline int b[] = { 8,8,8,8,8,8,8,8 };
	};

	class exp {
	public:
		const static inline int x[] = { 19,19,19,19,19,19,19,19 };
		const static inline int y[] = { 0,0,0,0,0,0,0,0 };
		const static inline int l[] = { 0,1,2,3,4,5,6,7 };
		const static inline int b[] = { 1,1,1,1,1,1,1,1 };
	};

	class pan {
	public:
		const static inline int x[] = { 3,3,3,3,3,3,3 };
		const static inline int y[] = { 3,3,3,3,3,3,3 };
		const static inline int l[] = { 1,2,3,4,5,6,7 };
		const static inline int b[] = { 1,1,1,1,1,1,1 };
	};

	class rev {
	public:
		const static inline int x[] = { 3,3,3,3,3,3,3,3 };
		const static inline int y[] = { 3,3,3,3,3,3,3,3 };
		const static inline int l[] = { 0,1,2,3,4,5,6,7 };
		const static inline int b[] = { 8,8,8,8,8,8,8,8 };
	};

	class cho {
	public:
		const static inline int x[] = { 3,3,3,3,3,3,3,3 };
		const static inline int y[] = { 3,3,3,3,3,3,3,3 };
		const static inline int l[] = { 0,1,2,3,4,5,6,7 };
		const static inline int b[] = { 4,4,4,4,4,4,4,4 };
	};

	class var {
	public:
		const static inline int x[] = { 3,3,3,3,3,3,3,3 };
		const static inline int y[] = { 3,3,3,3,3,3,3,3 };
		const static inline int l[] = { 0,1,2,3,4,5,6,7 };
		const static inline int b[] = { 2,2,2,2,2,2,2,2 };
	};

	class key {
	public:
		class sign {
		public:
			enum sign_e {
				vertical = 1,
				horizontal = 0
			};

			const static inline int x[] = { 3,3 };
			const static inline int y[] = { 2,2 };
			const static inline int l[] = { 0,0 };
			const static inline int b[] = { 4,8 };

		};

		class d1 {
		public:
			const static inline int x[] = { 3,3,3,3,3,3,3 };
			const static inline int y[] = { 2,2,2,2,2,3,2 };
			const static inline int l[] = { 1,1,7,1,1,0,1 };
			const static inline int b[] = { 8,4,8,8,8,1,8 };
		};

		class d0 {
		public:
			const static inline int x[] = { 3,3,3,3,3,3,3 };
			const static inline int y[] = { 2,2,2,2,2,2,2 };
			const static inline int l[] = { 6,6,3,2,2,7,3 };
			const static inline int b[] = { 4,8,8,8,4,4,4 };
		};
	};

	class arrow {
	public:

		enum arrow_e {
			midi = 0,
			bank,
			pgm,
			vol,
			exp,
			pan,
			rev,
			cho,
			var,
			key
		};

		const static inline int x[] = { 3,3,3,3,3,3,3,3,3,3 };
		const static inline int y[] = { 2,2,2,2,2,2,2,2,2,2 };
		const static inline int l[] = { 2,1,0,3,4,5,6,7,6,7 };
		const static inline int b[] = { 2,2,2,2,2,2,2,2,1,1 };
	};

	class mode {
	public:
		const static inline int x[] = { 3,3,3,3 };
		const static inline int y[] = { 2,2,2,2 };
		const static inline int l[] = { 4,4,5,5 };
		const static inline int b[] = { 4,8,4,8 };
	};


};