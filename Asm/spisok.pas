unit spisok;
interface
(*{$CODEPAGE UTF8}*)
uses module, types;

{return 2^n}
function stepen_2 ( n: byte ) : word;
{перевод в точку отрезка}
function osob_to_x( osob: word ) : real;
function F( osob: word ) : real;
{добавление в конец списка}
procedure add_tail( var tail: ref_list; osob: word );
{инициализация списка}
procedure init( var head, tail: ref_list; pop_volume: integer{ = 30} );
{освобождение памяти}
procedure destruct( var head, tail: ref_list );
{вставка b после a}
procedure insert( var a, b, head, tail: ref_list );
{количество элементов списка}
function num_of_all( head: ref_list ) : integer;
{сортировка вставками}
procedure sort_vst( var head, tail: ref_list );
{удаление элемента}
procedure remove( var head, tail, x: ref_list);
{удаление дубликатов}
procedure duplicate ( var head, tail: ref_list);
{дозаполнение}
procedure fill( var head, tail: ref_list; pop_volume: integer);
{вывод в двоичной системе}
procedure print_bin( osob: word );
{вывод}
procedure print( head: ref_list );

Implementation

{return 2^n}
function stepen_2 ( n: byte ) : word;
begin
	stepen_2 := round( exp( n * ln( 2 ) ) );
end;

{перевод в точку отрезка}
function osob_to_x( osob: word ) : real;
begin
	osob_to_x := osob * exp((2 - m) * ln(2));
end;

function F( osob: word ) : real;
var
	x: real;
begin
	x := osob_to_x( osob );
	F := (x - 2) * (x - 2.5) * (x - 3.5) * (1 - exp(x - 1.5));
end;

{добавление в конец списка}
procedure add_tail( var tail: ref_list; osob: word );
begin
	if tail = nil then
	begin
		writeln('ERROR_1');	{list empty}
		exit;
	end;
	new(tail^.next);
	tail^.next^.next := nil;
	tail^.next^.prev := tail;
	tail := tail^.next;
	tail^.osob := osob;
	tail^.quality := F( osob );
end;

{инициализация списка}
procedure init( var head, tail: ref_list; pop_volume: integer{ = 30} );
var
	osob: word;
	i: integer;
begin
	new(head);
	head^.next := nil;
	head^.prev := nil;
	tail := head;
	//randomize;
	RandSeed := 411;
	osob := random( round( exp( m * ln( 2 ) ) ) );
	head^.osob := osob;
	head^.quality := F( osob );
	for i := 1 to pop_volume - 1 do
	begin
		osob := random( round( exp( m * ln( 2 ) ) ) );
		add_tail( tail, osob );
	end;
end;

{освобождение памяти}
procedure destruct( var head, tail: ref_list );
var
	x: ref_list;
begin
	if head <> nil then
	begin
		x := head^.next;
		while x <> nil do
		begin
			dispose( x^.prev );
			x^.prev := nil;
			x := x^.next;
		end;
		dispose( tail );
		tail := nil;
		head := nil;
	end;
end;

{вставка b после a}
procedure insert( var a, b, head, tail: ref_list );
var
	an, bn, bp, temp: ref_list;
begin
	if a = nil then
	begin
		bn := b^.next;
		bp := b^.prev;
		b^.prev := nil;
		head^.prev := b;
		b^.next := head;
		if bn <> nil then
			bn^.prev := bp;
		bp^.next := bn;
		head := head^.prev;
		temp := head;
		while temp^.next <> nil do
			temp := temp^.next;
		tail := temp;
		exit;
	end;
	if a^.next = b then
		exit;
	an := a^.next;
	bn := b^.next;
	bp := b^.prev;
	a^.next := b;
	b^.prev := a;
	an^.prev := b;
	b^.next := an;
	if bn <> nil then
		bn^.prev := bp;
	bp^.next := bn;
	temp := head;
	while temp^.next <> nil do
		temp := temp^.next;
	tail := temp;
end;

{количество элементов списка}
function num_of_all( head: ref_list ) : integer;
var
	sum: integer;
	x: ref_list;
begin
	x := head;
	sum := 1;
	if x = nil then
	begin
		num_of_all := 0;
		exit;
	end;
	while x^.next <> nil do
	begin
		sum := sum + 1;
		x := x^.next;
	end;
	num_of_all := sum;
end;

{сортировка вставками}
procedure sort_vst( var head, tail: ref_list );
var 
	i, j: ref_list;
begin
	if head = tail then exit;
	i := head^.next;
	while i <> nil do begin
		j := i;
		while ( j <> head ) and ( j^.prev^.quality > i^.quality ) do
			j := j^.prev;
		insert( j^.prev, i, head, tail );
		i := i^.next;
	end;
end;

{удаление элемента}
procedure remove( var head, tail, x: ref_list);
var
	xp, xn, temp: ref_list;
begin
	if head = nil then
	begin
		writeln('ERROR_2');	{list empty}
		exit;
	end;
	if ( x = head ) and ( x = tail ) then begin
		dispose( x );
		head := nil;
		tail := nil;
		writeln('ERROR_4');	{list empty}
	end;
	if x = tail then begin
		dispose(x);
		x := nil;
		temp := head;
		while temp^.next <> nil do temp := temp^.next;
		tail := temp;
		exit;
	end;
	if x = head then begin
		dispose( x );
		x := nil;
		temp := tail;
		while temp^.prev <> nil do temp := temp^.prev;
		head := temp;
		exit;
	end;
	xp := x^.prev;
	xn := x^.next;
	dispose( x );
	x := nil;
	xp^.next := xn;
	xn^.prev := xp;
end;

{удаление дубликатов}
procedure duplicate( var head, tail: ref_list );
var
	x: ref_list;
begin
	if head = nil then
	begin
		writeln('ERROR_12');	{list empty}
		exit;
	end;
	sort_vst( head, tail );
	x := head;
	while x^.next <> nil do
	begin
		if x^.osob = x^.next^.osob then
		begin
			x := x^.next;
			remove( head, tail, x^.prev );
		end
		else x := x^.next;
	end;
end;

{дозаполнение}
procedure fill( var head, tail: ref_list; pop_volume: integer);
var
	i: integer;
	osob: word;
begin
	for i := 1 to ( pop_volume - num_of_all( head ) ) do
	begin
		osob := random( round( exp( m * ln( 2 ) ) ) );
		add_tail( tail, osob );
	end;
end;

{вывод в двоичной системе}
procedure print_bin( osob: word );
var
	i: byte;
	temp_osob: word;
	a: array [1..m] of byte;
begin
	temp_osob := osob;
	for i := 1 to m do
	begin
		a[i] := temp_osob mod 2;
		temp_osob := temp_osob div 2;
	end;
	for i := m downto 1 do
		write(a[i]);
end;

{вывод}
procedure print( head: ref_list );
var
	x: ref_list;
begin
	if head = nil then
	begin
		writeln('ERROR_5');	{list empty}
		exit;
	end;
	x := head;
	Write('Особь: '); print_bin( x^.osob );
	Writeln('   Точка: ', osob_to_x( x^.osob ) : 1 : 6);
	Writeln('Целевая функция: ', x^.quality : 1 : 6);
	Writeln();
	x := head^.next;
	while x <> nil do begin
		Write('Особь: '); print_bin( x^.osob );
		Writeln('   Точка: ', osob_to_x( x^.osob ) : 1 : 6);
		Writeln('Целевая функция: ', x^.quality : 1 : 6);
		Writeln();
		x := x^.next;
	end;
	writeln();
end;
begin end.
