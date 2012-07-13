#ifndef __MAP_H__
#define	__MAP_H__

#define				MAP_MASK_USE		0xFFFF
#define				MAP_FILE_MAGIC		0xA4D5400

#define				MAP_BLOCK_LEFT		0x80000
#define				MAP_BLOCK_RIGHT		0x20000
#define				MAP_BLOCK_TOP		0x10000
#define				MAP_BLOCK_DOWN		0x40000
#define				MAP_PROP_SLIPPERY	0x100000
#define				MAP_PROP_THICK		0x200000
#define				MAP_PROP_POISON		0x400000
#define				MAP_PROP_TELEPORTER	0x10000000
#define				MAP_PROP_RNDENC		0x20000000
#define				MAP_PROP_ACTION		0x40000000



typedef struct {
	unsigned int		magic;
	const char		internal_name[16];
	const char		music[32];
	const char		full_name[64];
	const char		tilesheet_file[32];
	const char		logic_lib[32];
	unsigned int		tile_w;
	unsigned int		tile_h;
	unsigned int		w;
	unsigned int		h;
	unsigned int		layers;
	unsigned int		teleporters;
	unsigned int		spawn_points;
	unsigned int		triggers;
} MAP_FILE_HEADER;


typedef struct {
	const char		target_map[16];
	unsigned int		target_x;
	unsigned int		target_y;
	unsigned int		target_layer;

	unsigned int		source_x;
	unsigned int		source_y;
	unsigned int		source_layer;
} MAP_FILE_TELEPORTER;


typedef struct {
	unsigned int		spawn_x;
	unsigned int		spawn_y;
	unsigned int		spawn_layer;

	const char		sprite_file[64];
	const char		logic_func[16];
} MAP_FILE_NPC;


typedef struct {
	unsigned int		x;
	unsigned int		y;
	unsigned int		layer;

	const char		logic_func[16];
} MAP_FILE_TRIGGER; 


typedef struct {
	DARNIT_TILEMAP		**layer;
	void			*tilesheet;
	int			layers;
	int			w;
	int			h;
	int			tile_w;
	int			tile_h;
	MAP_FILE_TELEPORTER	*teleporter;
	unsigned int		teleporters;
	MAP_FILE_TRIGGER	*trigger;
	unsigned int		triggers;
} MAP;


#endif
