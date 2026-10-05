#include "unknown_11c920.h"
#include "unknown_0259d0.h"
// @flags /O2 /arch:SSE /Gr

dword g_468848[256] =
{
	0xff7e7eff, 0xff7e7fff, 0xff7e80ff, 0xff7e81ff, 0xff7f7eff, 0xff7f7fff, 0xff7f80ff, 0xff7f81ff,
	0xff807eff, 0xff807fff, 0xff8080ff, 0xff8081ff, 0xff817eff, 0xff817fff, 0xff8180ff, 0xff8181ff,
	0xff7f82ff, 0xff837fff, 0xff7d7fff, 0xff8183ff, 0xff817cff, 0xff7c82ff, 0xff8481ff, 0xff7d7cff,
	0xff7f85ff, 0xff847dff, 0xff7a80ff, 0xff8484ff, 0xff807aff, 0xff7c85ff, 0xff877fff, 0xff7a7cff,
	0xff8288ff, 0xff8479ff, 0xff7883ff, 0xff8884ff, 0xff7c77ff, 0xff7d89ff, 0xff897bff, 0xff767dff,
	0xff8689ff, 0xff8275ff, 0xff7787ff, 0xff8c81ff, 0xff7877ff, 0xff808dff, 0xff8977ff, 0xff7381ff,
	0xff8b88ff, 0xff7e72ff, 0xff788cff, 0xff8e7cff, 0xff7379ff, 0xff858eff, 0xff8671ff, 0xff7187fe,
	0xff9085fe, 0xff7871fe, 0xff7c91fe, 0xff8e76fe, 0xff6e7efe, 0xff8c8efe, 0xff816dfe, 0xff728efe,
	0xff937ffe, 0xff7173fe, 0xff8394fe, 0xff8c6ffe, 0xff6b85fe, 0xff938bfe, 0xff796bfe, 0xff7794fe,
	0xff9577fd, 0xff6a78fd, 0xff8b95fd, 0xff8669fd, 0xff6c8dfd, 0xff9884fd, 0xff716cfd, 0xff7e99fd,
	0xff936ffd, 0xff6680fd, 0xff9392fd, 0xff7e65fd, 0xff6f96fd, 0xff9b7bfc, 0xff6871fc, 0xff879bfc,
	0xff8d67fc, 0xff658afc, 0xff9b8bfc, 0xff7365fc, 0xff779dfc, 0xff9b71fc, 0xff6279fc, 0xff929afc,
	0xff8460fb, 0xff6795fb, 0xffa181fb, 0xff6969fb, 0xff81a1fb, 0xff9666fb, 0xff5e84fb, 0xff9c94fb,
	0xff785efb, 0xff6e9ffb, 0xffa275fa, 0xff5f71fa, 0xff8ea2fa, 0xff8d5dfa, 0xff5f91fa, 0xffa48afa,
	0xff6c60fa, 0xff79a6fa, 0xff9f68f9, 0xff597df9, 0xff9b9df9, 0xff8058f9, 0xff659ef9, 0xffa97cf9,
	0xff5f67f9, 0xff87a9f8, 0xff975cf8, 0xff578bf8, 0xffa694f8, 0xff7157f8, 0xff6fa8f8, 0xffa86df8,
	0xff5673f7, 0xff96a7f7, 0xff8a54f7, 0xff5b9af7, 0xffae86f7, 0xff625cf7, 0xff7eaff7, 0xffa25ef6,
	0xff5082f6, 0xffa59ff6, 0xff7a50f6, 0xff64a8f6, 0xffb075f6, 0xff5567f5, 0xff8fb0f5, 0xff9552f5,
	0xff5194f5, 0xffb092f5, 0xff6852f4, 0xff72b2f4, 0xffac64f4, 0xff4c77f4, 0xffa1aaf4, 0xff854af4,
	0xff58a5f3, 0xffb780f3, 0xff575bf3, 0xff85b7f3, 0xffa254f3, 0xff498af2, 0xffb09ef2, 0xff7149f2,
	0xff65b3f2, 0xffb66cf2, 0xff4a6af1, 0xff99b5f1, 0xff9248f1, 0xff4c9ef1, 0xffbb8df0, 0xff5d4ff0,
	0xff78bcf0, 0xffaf59f0, 0xff427df0, 0xffacacef, 0xff7d42ef, 0xff58b0ef, 0xffbf78ef, 0xff4c5cee,
	0xff8ebfee, 0xffa048ee, 0xff4294ee, 0xffbb9ced, 0xff6743ed, 0xff69beed, 0xffbb61ed, 0xff3f6fed,
	0xffa4b9ec, 0xff8c3dec, 0xff4aaaec, 0xffc486eb, 0xff514deb, 0xff80c5eb, 0xffaf4deb, 0xff3a86ea,
	0xffb8abea, 0xff743aea, 0xff5abcea, 0xffc56de9, 0xff405fe9, 0xff99c4e9, 0xff9c3de9, 0xff3e9fe8,
	0xffc696e8, 0xff5b40e8, 0xff70c9e7, 0xffbd55e7, 0xff3576e7, 0xffb1bae7, 0xff8334e6, 0xff4ab6e6,
	0xffcd7de6, 0xff454ee5, 0xff8acde5, 0xffad40e5, 0xff3391e4, 0xffc4a7e4, 0xff6834e4, 0xff5ec8e3,
	0xffca61e3, 0xff3465e3, 0xffa5c8e3, 0xff9531e2, 0xff3bace2, 0xffd18ee2, 0xff4e3fe1, 0xff79d3e1,
	0xffbd48e1, 0xff2c80e0, 0xffbeb9e0, 0xff792ce0, 0xff4cc3df, 0xffd471df, 0xff3852df, 0xff96d3de,
	0xffa833de, 0xff2f9edd, 0xffd1a1dd, 0xff5b31dd, 0xff66d4dc, 0xffcc54dc, 0xff296ddc, 0xffb3c9db,
	0xff8c27db, 0xff3bbadb, 0xffda84da, 0xff4040da, 0xff84dbd9, 0xffbb3ad9, 0xff258cd9, 0xffcbb5d8,
	0xff6c26d8, 0xff52d0d8, 0xffd964d7, 0xff2b59d7, 0xffa4d7d6, 0xffa027d6, 0xff2cacd6, 0x008080ff,
};

dword __cdecl pack_color4f(color4f const *arg_1);
dword __cdecl pack_color3f(color3f const *arg_1);
real function_135880(word arg_1);

PRIVATE __forceinline real function_135ca1(real arg_1)
{
	real local_1 = arg_1 * 100.0f;
	return 0.0f > local_1 ? 0.0f : (local_1 > 1.0f ? 1.0f : local_1);
}

// @retail 0x135ca0
dword function_135ca0(long arg_1, short arg_2, void const *arg_3)
{
	switch (arg_2)
	{
	case 6:
		{
			dword local_1 = ((word const *)arg_3)[arg_1];
			return ((((((local_1 & 0xf800) | 0xffff0000) << 3) | (local_1 & 0x7e0)) << 2 | (local_1 & ~0x1fe0)) << 3)
				| ((((local_1 >> 1) & 0xe) | (local_1 & 0x600)) >> 1);
		}
	case 8:
		{
			dword local_1 = ((word const *)arg_3)[arg_1];
			return (((((local_1 & 0x7c00) << 9) | ((local_1 & 0x3e0) << 6)) | ((local_1 & 0x7000) << 4))
				| ((local_1 & 0x380) << 1) | ((local_1 & 0x1f) << 3) | ((local_1 >> 2) & 7)
				| ((local_1 >> 15) * 255 << 24));
		}
	case 9:
		{
			dword local_1 = ((word const *)arg_3)[arg_1];
			dword local_2 = (local_1 >> 8) & 15;
			dword local_3 = (local_1 >> 4) & 15;
			dword local_4 = local_1 & 15;
			return ((((((((((local_1 >> 8) & ~15) << 12) | local_1) & ~0xfff)
				| (((local_2 << 4) | local_2) << 4) | local_3) << 4 | local_3) << 4 | local_4) << 4) | local_4);
		}
	case 10:
	case 11:
		return ((dword const *)arg_3)[arg_1];
	case 0:
		return ((byte const *)arg_3)[arg_1] << 24;
	case 1:
		{
			dword local_1 = ((byte const *)arg_3)[arg_1];
			return (((0xffffff00 | local_1) << 8) | local_1) << 8 | local_1;
		}
	case 2:
		{
			dword local_1 = ((byte const *)arg_3)[arg_1];
			return (((((local_1 << 8) | local_1) << 8) | local_1) << 8) | local_1;
		}
	case 3:
		{
			word local_1 = ((word const *)arg_3)[arg_1];
			dword local_2 = (byte)local_1;
			return ((((local_1 & 0xffffff00) | local_2) << 8) | local_2) << 8 | local_2;
		}
	case 17:
		return g_468848[((byte const *)arg_3)[arg_1]];
	case 18:
		return ((byte const *)arg_3)[arg_1];
	case 19:
		{
			color4f local_1 = ((color4f const *)arg_3)[arg_1];
			local_1.alpha = function_135ca1(local_1.alpha);
			local_1.red = function_135ca1(local_1.red);
			local_1.green = function_135ca1(local_1.green);
			local_1.blue = function_135ca1(local_1.blue);
			return pack_color4f(&local_1);
		}
	case 20:
		{
			color3f local_1 = ((color3f const *)arg_3)[arg_1];
			local_1.red = function_135ca1(local_1.red);
			local_1.green = function_135ca1(local_1.green);
			local_1.blue = function_135ca1(local_1.blue);
			return pack_color3f(&local_1);
		}
	case 21:
		{
			struct s_135ca0 { word field_0; word field_2; word field_4; };
			s_135ca0 local_1 = ((s_135ca0 const *)arg_3)[arg_1];
			color4f local_2;
			local_2.alpha = 1.0f;
			local_2.red = function_135ca1(function_135880(local_1.field_0));
			local_2.green = function_135ca1(function_135880(local_1.field_2));
			local_2.blue = function_135ca1(function_135880(local_1.field_4));
			return pack_color4f(&local_2);
		}
	case 22:
		{
			dword local_1 = ((word const *)arg_3)[arg_1];
			return ((0xffffff00 | local_1) << 16) | (local_1 & 0xffffff00);
		}
	case 23:
		return ((word const *)arg_3)[arg_1] | 0xff000000;
	default:
		return 0;
	}
}
