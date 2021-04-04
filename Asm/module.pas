unit module;

interface
uses types;

{$L ./module_asm.obj}
{сортировка вставками}
procedure sort( var head: ref_list );
pascal; external name 'sort';
{одноточечное скрещивание}
function crossing_one( osob1, osob2: word ) : longword;
pascal; external name 'crossing_one';
{двухточечное скрещивание}
function crossing_two( osob1, osob2: word ) : longword;
pascal; external name 'crossing_two';
{универсальное скрещивание}
function crossing_universal( osob1, osob2: word; universal_chance: 
word ) : longword; pascal; external name 'crossing_universal';
{однородное скрещивание}
function crossing_odnor( osob1, osob2: word ): word;
pascal; external name 'crossing_odnor';
{мутация заменой}
function mutation_change ( osob: word; change_chance: word ) : word;
pascal; external name 'mutation_change';
{мутация перестановкой}
function mutation_swap( osob: word ) : word;
pascal; external name 'mutation_swap';
{мутация реверсом}
function mutation_reverse( osob: word ) : word;
pascal; external name 'mutation_reverse';

implementation
(*
{сортировка вставками}
procedure sort( var head: ref_list );
var 
	i, j: ref_list;
begin
	i := head^.next;
	while i <> nil do begin
		j := i;
		while ( j <> head ) and ( j^.prev^.quality > i^.quality ) do
			j := j^.prev;
		insert( j^.prev, i, head, tail );
		i := i^.next;
	end;
end;

{одноточечное скрещивание}
function crossing_one( osob1, osob2: word ) : longword;
var
	n: byte;					{точка}
	mask_x, mask_y, new_osob: word;
begin
	n := random( 15 ) + 1;		{выбор точки}
	mask_y := stepen_2( n ) - 1;
	mask_x := max_word - mask_y;
	new_osob := ( osob1 and mask_x ) + ( osob2 and mask_y );
	//add_tail( tail, new_osob );
	new_osob := ( osob2 and mask_x ) + ( osob1 and mask_y );
	//add_tail( tail, new_osob );
end;

{двухточечное скрещивание}
function crossing_two( osob1, osob2: word ) : longword;
var
	n1, n2: byte;				{точки}
	mask_x, mask_y, new_osob: word;
begin
	n2 := random( m - 2 ) + 1;		{выбор точек}
	n1 := n2 + random( m - n2 ) + 1;
	mask_y := stepen_2( n1 ) - stepen_2( n2 );
	mask_x := max_word - stepen_2( n1 ) + stepen_2( n2 );
	new_osob := ( osob1 and mask_x ) + ( osob2 and mask_y );
	//add_tail( tail, new_osob );
	new_osob := ( osob2 and mask_x ) + ( osob1 and mask_y );
	//add_tail( tail, new_osob );
end;

{универсальное скрещивание}
function crossing_universal( osob1, osob2: word;
									universal_chance: word ) : longword;
var
	i, shot: byte;
	new1, new2: word;
begin
	new1 := 0;
	new2 := 0;
	for i := 1 to m do
	begin
		shot := random( 100 ) + 1;
		if ( universal_chance ) > shot then	{есть попадание}
		begin
			new2 := new2 + ( osob1 and stepen_2( i - 1 ) );
			new1 := new1 + ( osob2 and stepen_2( i - 1 ) );
		end
		else
		begin
			new2 := new2 + ( osob2 and stepen_2( i - 1 ) );
			new1 := new1 + ( osob1 and stepen_2( i - 1 ) );
		end;
	end;
	//add_tail( tail, new1 );
	//add_tail( tail, new2 );
end;

{однородное скрещивание}
function crossing_odnor( osob1, osob2: word ) : word;
var
	mask, new_osob: word;
begin
	mask := random( max_word + 1 );	{случайная маска}
	new_osob := ( osob1 and mask )	{единичные из первой}
						+ ( osob2 and (not mask) ); {нулевые из второй}
	//add_tail( tail, new_osob );
end;

{мутация заменой}
function mutation_change ( osob: word; change_chance: word ) : word;
var
	new_osob: word;
	shot, i: byte;
begin
	new_osob := 0;
	for i := 1 to m do
	begin
		shot := random( 100 ) + 1;
		if ( 100 * change_chance ) > shot then	{есть попадание}
			new_osob := new_osob + ( osob xor stepen_2( i - 1 ) )
		else new_osob := new_osob + ( osob and stepen_2( i - 1 ) );
	end;
	//add_tail( tail, new_osob );
end;

{мутация перестановкой}
function mutation_swap( osob: word ) : word;
var
	new_osob: word;
	a, b: byte;						{биты для swap}
begin
	a := random( m ) + 1;
	b := random( m ) + 1;
	while b = a do
		b := random( m ) + 1;
	new_osob := swap( osob, a, b );
	//add_tail( tail, new_osob );
end;

{мутация реверсом}
function mutation_reverse( osob: word ) : word;
var
	new_osob: word;
	a, b: byte;
begin
	new_osob := osob;
	a := random( m - 1 ) + 2; 		{точка реверсии}
	b := 1;
	while a > b do
	begin
		new_osob := swap( new_osob, a, b );
		a := a - 1;
		b := b + 1;
	end;
	//add_tail( tail, new_osob );
end;
*)
begin
end.
