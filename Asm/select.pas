unit select;
interface
(*{$CODEPAGE UTF8}*)
uses spisok, types;

{среднее значение функции}
function f_ave( head: ref_list ) : real;
{расставляет вероятности}
procedure chances( head: ref_list );
{общее кол-во целых шансов до границы (для рандома)}
function chances_before( head: ref_list; border: integer ) : integer;
{пропорциональный отбор}
procedure selection_proporc( var head, tail: ref_list; preserv_high,
								preserv_low, kill_proporc: integer );
{отбор усечением}
procedure selection_usech( var head, tail: ref_list;
				preserv_high, preserv_low: integer; porog_usech: real );

Implementation

{среднее значение функции}
function f_ave( head: ref_list ) : real;
var
	x: ref_list;
	sum: real;
begin
	if head = nil then
	begin
		writeln('ERROR_6');	{list empty}
		f_ave := -1;
		exit;
	end;
	x := head;
	sum := 0.0;
	while x <> nil do
	begin
		sum := sum + x^.quality;
		x := x^.next;
	end;
	f_ave := sum / num_of_all( head );
end;

{расставляет вероятности}
procedure chances( head: ref_list );
var
	x: ref_list;
	ave: real;
begin
	if head = nil then
	begin
		writeln('ERROR_7');	{list empty}
		exit;
	end;
	x := head;
	ave := f_ave( head );
	while x <> nil do
	begin
		x^.chance := round( abs( x^.quality / ave ));
		if x^.chance = 0 then
			x^.chance := 1;
		x := x^.next;
	end;
end;

{общее кол-во целых шансов до границы (для рандома)}
function chances_before( head: ref_list; border: integer ) : integer;
var
	x: ref_list;
	sum, count: integer;
begin
	if head = nil then
	begin
		writeln('ERROR_9');	{list empty}
		chances_before := -1;
		exit;
	end;
	x := head;
	sum := 0;
	count := 1;
	while ( x <> nil ) and ( count <= border ) do
	begin
		sum := sum + x^.chance;
		count := count + 1;
		x := x^.next;
	end;
	if x = nil then
	begin
		writeln('ERROR_10');{not reached}
		chances_before := -1;
	end
	else chances_before := sum;
end;

{пропорциональный отбор}
procedure selection_proporc( var head, tail: ref_list; preserv_high,
								preserv_low, kill_proporc: integer );
var
	x: ref_list;
	shot, i, left_border, right_border, j: integer;
begin
	if head = nil then
	begin
		writeln('ERROR_8');	{list empty}
		exit;
	end;
	chances( head );
	for i := 1 to kill_proporc do
	begin
		left_border := chances_before( head, preserv_low );
		right_border := chances_before( head, ( num_of_all( head )
													- preserv_high ) );
		if left_border >= right_border then
		begin
			writeln('Неверные значения на сохраняемые особи или ',
				'неверное число повторений kill_proporc, стоп машина');
			exit;
		end;
		shot := left_border + random( right_border - left_border ) + 1;
		x := head;
		j := head^.chance;
		while ( x <> nil ) and ( j < shot ) do
		begin
			x := x^.next;
			j := j + x^.chance;
		end;
		if x = nil then
			writeln('ERROR_11')	{not reached}
		else remove( head, tail, x );
	end;
end;

{отбор усечением}
procedure selection_usech( var head, tail: ref_list;
				preserv_high, preserv_low: integer; porog_usech: real );
var
	num, i: integer;
	x: ref_list;
begin
	x := head;
	for i := 1 to ( preserv_low - 1 ) do
		x := x^.next;
	num := trunc( num_of_all( head ) * porog_usech );
	if num + preserv_high + preserv_low > num_of_all( head ) then
	begin
		writeln('Неверные значения на сохраняемые особи или ',
				'неверный порог porog_usech, стоп машина');
		exit;
	end;
	for i := 1 to num do
		remove( head, tail, x^.next );
end;

begin end.
