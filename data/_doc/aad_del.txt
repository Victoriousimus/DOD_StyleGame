--------------------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 0 ]; [ vert_count : 6 ]; ------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 6 ]; [ vert_count : 36 ]; ------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 42 ]; [ vert_count : 120 ]; ------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 162 ]; [ vert_count : 10839 ]; ------------------------------------

--------------------------------------------------
n0:
	cmd_lim = 5
	atr_lim = 5
	ent[e]:
		[0] : Enty_pos; mesh_id = 1; inst_id = -1;
		[1] : Enty_pos; mesh_id = 2; inst_id =  2;
		[2] : Enty_pos; mesh_id = 3; inst_id =  4;
		[3] : Enty_pos; mesh_id = 1; inst_id =  0;
		[4] : Enty_pos; mesh_id = 2; inst_id =  1;
		[5] : Enty_pos; mesh_id = 3; inst_id =  3;
		[6] : Enty_pos; mesh_id = 1; inst_id = -1;
		[7] : Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 1 ];
		[2] :  [ inst_first : 1 ];  [ inst_count : 2 ];
		[3] :  [ inst_first : 3 ];  [ inst_count : 2 ];
		[4] :  [ inst_first : 5 ];  [ inst_count : 0 ];
	atr[a]:
		[0] : ent[3].Enty_pos;
		[1] : ent[4].Enty_pos;
		[2] : ent[1].Enty_pos;
		[3] : ent[5].Enty_pos;
		[4] : ent[2].Enty_pos;
		[5] : 0;
		[6] : 0;
		[7] : 0;
n1:
	cmd_lim = 5
	atr_lim = 4(-1) //3
	ent[e]:
		[0] : Enty_pos; mesh_id = 1; inst_id = -1;
		[1] : Enty_pos; mesh_id = 2; inst_id =  1; ^
		[2] : Enty_pos; mesh_id = 3; inst_id =  3; ^
		[3] : Enty_pos; mesh_id = 1; inst_id = -1; < (c = 1, a = 0)
		[4] : Enty_pos; mesh_id = 2; inst_id =  0; ^
		[5] : Enty_pos; mesh_id = 3; inst_id =  2; ^
		[6] : Enty_pos; mesh_id = 1; inst_id = -1;
		[7] : Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 0 ]; <
		[2] :  [ inst_first : 0 ];  [ inst_count : 2 ]; ^
		[3] :  [ inst_first : 2 ];  [ inst_count : 2 ]; ^
		[4] :  [ inst_first : 4 ];  [ inst_count : 0 ]; ^
	atr[a]:
		[0] : ent[4].Enty_pos; ^
		[1] : ent[1].Enty_pos; ^
		[2] : ent[5].Enty_pos; ^
		[3] : ent[2].Enty_pos; ^
		[4] : 0;
		[5] : 0;
		[6] : 0;
		[7] : 0;
		
		
		
***************************************************************************	
		
0:
	cmd_lim = 5
	atr_lim = 0
	ent[e]:
		[0] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[1] :  Enty_pos; mesh_id = 2; inst_id = -1;
		[2] :  Enty_pos; mesh_id = 3; inst_id = -1;
		[3] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[4] :  Enty_pos; mesh_id = 2; inst_id = -1;
		[5] :  Enty_pos; mesh_id = 3; inst_id = -1;
		[6] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[7] :  Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[2] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[3] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[4] :  [ inst_first : 0 ];  [ inst_count : 0 ];
	atr[a]:
		[0] : 0;
		[1] : 0;
		[2] : 0;
		[3] : 0;
		[4] : 0;
		[5] : 0;
		[6] : 0;
		[7] : 0;
1:
	cmd_lim = 5
	atr_lim = 1(+1)
	ent[e]:
		[0] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[1] :  Enty_pos; mesh_id = 2; inst_id =  0; < (a, c = mesh_id)
		[2] :  Enty_pos; mesh_id = 3; inst_id = -1;
		[3] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[4] :  Enty_pos; mesh_id = 2; inst_id = -1;
		[5] :  Enty_pos; mesh_id = 3; inst_id = -1;
		[6] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[7] :  Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[2] :  [ inst_first : 0 ];  [ inst_count : 1 ]; < (a = inst_first)
		[3] :  [ inst_first : 1 ];  [ inst_count : 0 ]; ^ (+1)
		[4] :  [ inst_first : 1 ];  [ inst_count : 0 ]; ^ (+1)
	atr[a]:
		[0] : ent[1].Enty_pos; < (a = inst_first)
		[1] : 0;
		[2] : 0;
		[3] : 0;
		[4] : 0;
		[5] : 0;
		[6] : 0;
		[7] : 0;
2:
	cmd_lim = 5
	atr_lim = 2(+1)
	ent[e]:
		[0] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[1] :  Enty_pos; mesh_id = 2; inst_id =  0;
		[2] :  Enty_pos; mesh_id = 3; inst_id =  1; <  (a, c = mesh_id)
		[3] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[4] :  Enty_pos; mesh_id = 2; inst_id = -1;
		[5] :  Enty_pos; mesh_id = 3; inst_id = -1;
		[6] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[7] :  Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[2] :  [ inst_first : 0 ];  [ inst_count : 1 ]; 
		[3] :  [ inst_first : 1 ];  [ inst_count : 1 ]; < (a = inst_first)
		[4] :  [ inst_first : 2 ];  [ inst_count : 0 ]; ^ (+1)
	atr[a]:
		[0] : ent[1].Enty_pos;
		[1] : ent[2].Enty_pos; < (a = inst_first)
		[2] : 0;
		[3] : 0;
		[4] : 0;
		[5] : 0;
		[6] : 0;
		[7] : 0;
3:
	cmd_lim = 5
	atr_lim = 3(+1)
	ent[e]:
		[0] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[1] :  Enty_pos; mesh_id = 2; inst_id =  1; ^ (new a)
		[2] :  Enty_pos; mesh_id = 3; inst_id =  2; ^ (new a)
		[3] :  Enty_pos; mesh_id = 1; inst_id =  0; < (a, c = mesh_id)
		[4] :  Enty_pos; mesh_id = 2; inst_id = -1;
		[5] :  Enty_pos; mesh_id = 3; inst_id = -1;
		[6] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[7] :  Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 1 ]; < (a = inst_first)
		[2] :  [ inst_first : 1 ];  [ inst_count : 1 ]; ^ (+1)
		[3] :  [ inst_first : 2 ];  [ inst_count : 1 ]; ^ (+1)
		[4] :  [ inst_first : 3 ];  [ inst_count : 0 ]; ^ (+1)
	atr[a]:
		[0] : ent[3].Enty_pos; < (a = inst_first)
		[1] : ent[1].Enty_pos; ^
		[2] : ent[2].Enty_pos; ^
		[3] : 0;
		[4] : 0;
		[5] : 0;
		[6] : 0;
		[7] : 0;
4:
	cmd_lim = 5
	atr_lim = 4(+1)
	ent[e]:
		[0] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[1] :  Enty_pos; mesh_id = 2; inst_id =  2; ^ (new a)
		[2] :  Enty_pos; mesh_id = 3; inst_id =  3; ^ (new a)
		[3] :  Enty_pos; mesh_id = 1; inst_id =  0; 
		[4] :  Enty_pos; mesh_id = 2; inst_id =  1; < (a, c = mesh_id)
		[5] :  Enty_pos; mesh_id = 3; inst_id = -1;
		[6] :  Enty_pos; mesh_id = 1; inst_id = -1;
		[7] :  Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 1 ];
		[2] :  [ inst_first : 1 ];  [ inst_count : 2 ]; < (a = inst_first)
		[3] :  [ inst_first : 3 ];  [ inst_count : 1 ]; ^ (+1)
		[4] :  [ inst_first : 4 ];  [ inst_count : 0 ]; ^ (+1)
	atr[a]:
		[0] : ent[3].Enty_pos;
		[1] : ent[4].Enty_pos; < (a = inst_first)
		[2] : ent[1].Enty_pos; ^
		[3] : ent[2].Enty_pos; ^
		[4] : 0;
		[5] : 0;
		[6] : 0;
		[7] : 0;
5:
	cmd_lim = 5
	atr_lim = 5(+1)
	ent[e]:
		[0] : Enty_pos; mesh_id = 1; inst_id = -1;
		[1] : Enty_pos; mesh_id = 2; inst_id =  2;
		[2] : Enty_pos; mesh_id = 3; inst_id =  4; ^ (new a)
		[3] : Enty_pos; mesh_id = 1; inst_id =  0; 
		[4] : Enty_pos; mesh_id = 2; inst_id =  1;
		[5] : Enty_pos; mesh_id = 3; inst_id =  3; < (a, c = mesh_id)
		[6] : Enty_pos; mesh_id = 1; inst_id = -1;
		[7] : Enty_pos; mesh_id = 2; inst_id = -1;
	cmd[c]:
		[0] :  [ inst_first : 0 ];  [ inst_count : 0 ];
		[1] :  [ inst_first : 0 ];  [ inst_count : 1 ];
		[2] :  [ inst_first : 1 ];  [ inst_count : 2 ];
		[3] :  [ inst_first : 3 ];  [ inst_count : 2 ]; < (a = inst_first)
		[4] :  [ inst_first : 5 ];  [ inst_count : 0 ]; ^ (+1)
	atr[a]:
		[0] : ent[3].Enty_pos;
		[1] : ent[4].Enty_pos;
		[2] : ent[1].Enty_pos;
		[3] : ent[5].Enty_pos; < (a = inst_first)
		[4] : ent[2].Enty_pos; ^
		[5] : 0;
		[6] : 0;
		[7] : 0;
		
--------------------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 0 ]; [ vert_count : 6 ]; ------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 6 ]; [ vert_count : 36 ]; ------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 42 ]; [ vert_count : 120 ]; ------------------------------------

------------------------------------
[ inst_first : 0 ]; [ inst_count : 0 ]; [ vert_first : 162 ]; [ vert_count : 10839 ]; ------------------------------------

--------------------------------------------------



OFFLINE_STEP [ 1] ---------------------------
ENTITY_LIMIT : 16
    [0] : mesh_id = 1; inst_id = -1;
    [1] : mesh_id = 2; inst_id = 0;
    [2] : mesh_id = 3; inst_id = -1;
    [3] : mesh_id = 1; inst_id = -1;
    [4] : mesh_id = 2; inst_id = -1;
    [5] : mesh_id = 3; inst_id = -1;
    [6] : mesh_id = 1; inst_id = -1;
    [7] : mesh_id = 2; inst_id = -1;
    [8] : mesh_id = 3; inst_id = -1;
    [9] : mesh_id = 1; inst_id = -1;
    [10] : mesh_id = 2; inst_id = -1;
    [11] : mesh_id = 3; inst_id = -1;
    [12] : mesh_id = 1; inst_id = -1;
    [13] : mesh_id = 2; inst_id = -1;
    [14] : mesh_id = 3; inst_id = -1;
    [15] : mesh_id = 1; inst_id = -1;

CMD_COUNT : 4
    [0] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 0 ];     [ vert_count : 6 ];
    [1] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 6 ];     [ vert_count : 36 ];
    [2] :  [ inst_first : 0 ];  [ inst_count : 1 ];     [ vert_first : 42 ];    [ vert_count : 120 ];
    [3] :  [ inst_first : 1 ];  [ inst_count : 0 ];     [ vert_first : 162 ];   [ vert_count : 10839 ];

INS_COUNT : 1
    [0] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.924;        y       = -1.072;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;


OFFLINE_STEP [ 2] ---------------------------
ENTITY_LIMIT : 16
    [0] : mesh_id = 1; inst_id = -1;
    [1] : mesh_id = 2; inst_id = 0;
    [2] : mesh_id = 3; inst_id = 1;
    [3] : mesh_id = 1; inst_id = -1;
    [4] : mesh_id = 2; inst_id = -1;
    [5] : mesh_id = 3; inst_id = -1;
    [6] : mesh_id = 1; inst_id = -1;
    [7] : mesh_id = 2; inst_id = -1;
    [8] : mesh_id = 3; inst_id = -1;
    [9] : mesh_id = 1; inst_id = -1;
    [10] : mesh_id = 2; inst_id = -1;
    [11] : mesh_id = 3; inst_id = -1;
    [12] : mesh_id = 1; inst_id = -1;
    [13] : mesh_id = 2; inst_id = -1;
    [14] : mesh_id = 3; inst_id = -1;
    [15] : mesh_id = 1; inst_id = -1;

CMD_COUNT : 4
    [0] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 0 ];     [ vert_count : 6 ];
    [1] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 6 ];     [ vert_count : 36 ];
    [2] :  [ inst_first : 0 ];  [ inst_count : 1 ];     [ vert_first : 42 ];    [ vert_count : 120 ];
    [3] :  [ inst_first : 1 ];  [ inst_count : 1 ];     [ vert_first : 162 ];   [ vert_count : 10839 ];

INS_COUNT : 2
    [0] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.924;        y       = -1.072;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [1] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.654;        y       = 9.922;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;


OFFLINE_STEP [ 3] ---------------------------
ENTITY_LIMIT : 16
    [0] : mesh_id = 1; inst_id = -1;
    [1] : mesh_id = 2; inst_id = 1;
    [2] : mesh_id = 3; inst_id = 2;
    [3] : mesh_id = 1; inst_id = 0;
    [4] : mesh_id = 2; inst_id = -1;
    [5] : mesh_id = 3; inst_id = -1;
    [6] : mesh_id = 1; inst_id = -1;
    [7] : mesh_id = 2; inst_id = -1;
    [8] : mesh_id = 3; inst_id = -1;
    [9] : mesh_id = 1; inst_id = -1;
    [10] : mesh_id = 2; inst_id = -1;
    [11] : mesh_id = 3; inst_id = -1;
    [12] : mesh_id = 1; inst_id = -1;
    [13] : mesh_id = 2; inst_id = -1;
    [14] : mesh_id = 3; inst_id = -1;
    [15] : mesh_id = 1; inst_id = -1;

CMD_COUNT : 4
    [0] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 0 ];     [ vert_count : 6 ];
    [1] :  [ inst_first : 0 ];  [ inst_count : 1 ];     [ vert_first : 6 ];     [ vert_count : 36 ];
    [2] :  [ inst_first : 1 ];  [ inst_count : 1 ];     [ vert_first : 42 ];    [ vert_count : 120 ];
    [3] :  [ inst_first : 2 ];  [ inst_count : 1 ];     [ vert_first : 162 ];   [ vert_count : 10839 ];

INS_COUNT : 3
    [0] :  yaw = 0;     pich = 0;       roll = 0;       x       = -0.346;       y       = 0.872;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [1] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.924;        y       = -1.072;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [2] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.654;        y       = 9.922;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;


OFFLINE_STEP [ 4] ---------------------------
ENTITY_LIMIT : 16
    [0] : mesh_id = 1; inst_id = -1;
    [1] : mesh_id = 2; inst_id = 2;
    [2] : mesh_id = 3; inst_id = 3;
    [3] : mesh_id = 1; inst_id = 0;
    [4] : mesh_id = 2; inst_id = 1;
    [5] : mesh_id = 3; inst_id = -1;
    [6] : mesh_id = 1; inst_id = -1;
    [7] : mesh_id = 2; inst_id = -1;
    [8] : mesh_id = 3; inst_id = -1;
    [9] : mesh_id = 1; inst_id = -1;
    [10] : mesh_id = 2; inst_id = -1;
    [11] : mesh_id = 3; inst_id = -1;
    [12] : mesh_id = 1; inst_id = -1;
    [13] : mesh_id = 2; inst_id = -1;
    [14] : mesh_id = 3; inst_id = -1;
    [15] : mesh_id = 1; inst_id = -1;

CMD_COUNT : 4
    [0] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 0 ];     [ vert_count : 6 ];
    [1] :  [ inst_first : 0 ];  [ inst_count : 1 ];     [ vert_first : 6 ];     [ vert_count : 36 ];
    [2] :  [ inst_first : 1 ];  [ inst_count : 2 ];     [ vert_first : 42 ];    [ vert_count : 120 ];
    [3] :  [ inst_first : 3 ];  [ inst_count : 1 ];     [ vert_first : 162 ];   [ vert_count : 10839 ];

INS_COUNT : 4
    [0] :  yaw = 0;     pich = 0;       roll = 0;       x       = -0.346;       y       = 0.872;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [1] :  yaw = 0;     pich = 0;       roll = 0;       x       = -9.694;       y       = -9.416;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [2] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.924;        y       = -1.072;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [3] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.654;        y       = 9.922;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;


OFFLINE_STEP [ 5] ---------------------------
ENTITY_LIMIT : 16
    [0] : mesh_id = 1; inst_id = -1;
    [1] : mesh_id = 2; inst_id = 2;
    [2] : mesh_id = 3; inst_id = 4;
    [3] : mesh_id = 1; inst_id = 0;
    [4] : mesh_id = 2; inst_id = 1;
    [5] : mesh_id = 3; inst_id = 3;
    [6] : mesh_id = 1; inst_id = -1;
    [7] : mesh_id = 2; inst_id = -1;
    [8] : mesh_id = 3; inst_id = -1;
    [9] : mesh_id = 1; inst_id = -1;
    [10] : mesh_id = 2; inst_id = -1;
    [11] : mesh_id = 3; inst_id = -1;
    [12] : mesh_id = 1; inst_id = -1;
    [13] : mesh_id = 2; inst_id = -1;
    [14] : mesh_id = 3; inst_id = -1;
    [15] : mesh_id = 1; inst_id = -1;

CMD_COUNT : 4
    [0] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 0 ];     [ vert_count : 6 ];
    [1] :  [ inst_first : 0 ];  [ inst_count : 1 ];     [ vert_first : 6 ];     [ vert_count : 36 ];
    [2] :  [ inst_first : 1 ];  [ inst_count : 2 ];     [ vert_first : 42 ];    [ vert_count : 120 ];
    [3] :  [ inst_first : 3 ];  [ inst_count : 2 ];     [ vert_first : 162 ];   [ vert_count : 10839 ];

INS_COUNT : 5
    [0] :  yaw = 0;     pich = 0;       roll = 0;       x       = -0.346;       y       = 0.872;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [1] :  yaw = 0;     pich = 0;       roll = 0;       x       = -9.694;       y       = -9.416;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [2] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.924;        y       = -1.072;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [3] :  yaw = 0;     pich = 0;       roll = 0;       x       = 9.436;        y       = 9.79;         z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [4] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.654;        y       = 9.922;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;


OFFLINE_STEP [ 6] ---------------------------
ENTITY_LIMIT : 16
    [0] : mesh_id = 1; inst_id = -1;
    [1] : mesh_id = 2; inst_id = 3;
    [2] : mesh_id = 3; inst_id = 5;
    [3] : mesh_id = 1; inst_id = 1;
    [4] : mesh_id = 2; inst_id = 2;
    [5] : mesh_id = 3; inst_id = 4;
    [6] : mesh_id = 1; inst_id = 0;
    [7] : mesh_id = 2; inst_id = -1;
    [8] : mesh_id = 3; inst_id = -1;
    [9] : mesh_id = 1; inst_id = -1;
    [10] : mesh_id = 2; inst_id = -1;
    [11] : mesh_id = 3; inst_id = -1;
    [12] : mesh_id = 1; inst_id = -1;
    [13] : mesh_id = 2; inst_id = -1;
    [14] : mesh_id = 3; inst_id = -1;
    [15] : mesh_id = 1; inst_id = -1;

CMD_COUNT : 4
    [0] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 0 ];     [ vert_count : 6 ];
    [1] :  [ inst_first : 0 ];  [ inst_count : 2 ];     [ vert_first : 6 ];     [ vert_count : 36 ];
    [2] :  [ inst_first : 2 ];  [ inst_count : 2 ];     [ vert_first : 42 ];    [ vert_count : 120 ];
    [3] :  [ inst_first : 4 ];  [ inst_count : 2 ];     [ vert_first : 162 ];   [ vert_count : 10839 ];

INS_COUNT : 6
    [0] :  yaw = 0;     pich = 0;       roll = 0;       x       = -6.924;       y       = -6.262;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [1] :  yaw = 0;     pich = 0;       roll = 0;       x       = -0.346;       y       = 0.872;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [2] :  yaw = 0;     pich = 0;       roll = 0;       x       = -9.694;       y       = -9.416;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [3] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.924;        y       = -1.072;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [4] :  yaw = 0;     pich = 0;       roll = 0;       x       = 9.436;        y       = 9.79;         z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [5] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.654;        y       = 9.922;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;


OFFLINE_STEP [ 7] ---------------------------
ENTITY_LIMIT : 16
    [0] : mesh_id = 1; inst_id = -1;
    [1] : mesh_id = 2; inst_id = 4;
    [2] : mesh_id = 3; inst_id = 6;
    [3] : mesh_id = 1; inst_id = 1;
    [4] : mesh_id = 2; inst_id = 3;
    [5] : mesh_id = 3; inst_id = 5;
    [6] : mesh_id = 1; inst_id = 0;
    [7] : mesh_id = 2; inst_id = 2;
    [8] : mesh_id = 3; inst_id = -1;
    [9] : mesh_id = 1; inst_id = -1;
    [10] : mesh_id = 2; inst_id = -1;
    [11] : mesh_id = 3; inst_id = -1;
    [12] : mesh_id = 1; inst_id = -1;
    [13] : mesh_id = 2; inst_id = -1;
    [14] : mesh_id = 3; inst_id = -1;
    [15] : mesh_id = 1; inst_id = -1;

CMD_COUNT : 4
    [0] :  [ inst_first : 0 ];  [ inst_count : 0 ];     [ vert_first : 0 ];     [ vert_count : 6 ];
    [1] :  [ inst_first : 0 ];  [ inst_count : 2 ];     [ vert_first : 6 ];     [ vert_count : 36 ];
    [2] :  [ inst_first : 2 ];  [ inst_count : 3 ];     [ vert_first : 42 ];    [ vert_count : 120 ];
    [3] :  [ inst_first : 5 ];  [ inst_count : 2 ];     [ vert_first : 162 ];   [ vert_count : 10839 ];

INS_COUNT : 7
    [0] :  yaw = 0;     pich = 0;       roll = 0;       x       = -6.924;       y       = -6.262;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [1] :  yaw = 0;     pich = 0;       roll = 0;       x       = -0.346;       y       = 0.872;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [2] :  yaw = 0;     pich = 0;       roll = 0;       x       = 4.07;         y       = 9.788;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [3] :  yaw = 0;     pich = 0;       roll = 0;       x       = -9.694;       y       = -9.416;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [4] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.924;        y       = -1.072;       z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [5] :  yaw = 0;     pich = 0;       roll = 0;       x       = 9.436;        y       = 9.79;         z       = 2;   Atlas_X = 0;     Atlas_Y = 0;
    [6] :  yaw = 0;     pich = 0;       roll = 0;       x       = 3.654;        y       = 9.922;        z       = 2;   Atlas_X = 0;     Atlas_Y = 0;

C:\Users\MYPC\Documents\Visual Studio 2022\_Projects\DOD_Style_Project\x64\Debug\DOD_Style_Project.exe (процесс 6412) завершил работу с кодом 0 (0x0).
Нажмите любую клавишу, чтобы закрыть это окно: