{by Denis Kozub}
program gen_algorithm;
(*{$CODEPAGE UTF8}*)
uses select, spisok, module, types, sysutils;
label next_line;
var
	x, head, tail: ref_list;
	pop_volume, preserv_high, preserv_low, i, shot1, shot2, num, j, k,
						kill_proporc, max_iter, povtor: integer;
	porog_usech, universal_chance, change_chance, variability, prev_q,
			crossing_volume, znach, quality_epsilon, enough_value: real;
	select_mode, cross_mode, mutate_mode,
								max_valueless_iters, test: byte;
	osob1, osob2: word;
	f: text;
	c: char;
	s, s1: string;
	flag: boolean;
	start, finish: qword;
	temp: longword;
	new_osob1, new_osob2: word;
{----------------------------------------------------------------------}

function prisvaivanie ( s: string; znach: real ) : boolean;
begin
	case s of
	'op_volume': pop_volume := round( znach );
	'reserv_high': preserv_high := round( znach );
	'reserv_low': preserv_low := round( znach );
	'ill_proporc': kill_proporc := round( znach );
	'elect_mode': select_mode := round( znach );
	'ross_mode': cross_mode := round( znach );
	'utate_mode': mutate_mode := round( znach );
	'ax_valueless_iters': max_valueless_iters := round( znach );
	'ax_iter': max_iter := round( znach );
	'orog_usech': porog_usech := znach;
	'niversal_chance': universal_chance := znach;
	'hange_chance': change_chance := znach;
	'uality_epsilon': quality_epsilon := znach;
	'nough_value': enough_value := znach;
	'ariability': variability := znach;
	'rossing_volume': crossing_volume := znach;
	else begin
		Writeln('Wrong config file!');
		prisvaivanie := false;
		exit;
	end;
	prisvaivanie := true;
	end;
end;

procedure selection();
begin
	case select_mode of
	1: selection_proporc( head, tail, preserv_high,
										preserv_low, kill_proporc );
	2: selection_usech( head, tail, preserv_high,
										preserv_low, porog_usech );
	end;
end;

procedure crossing( o1, o2: word );
begin
	case cross_mode of
	1: begin
		temp := crossing_one( o1, o2 );
		new_osob1 := (temp and max_word);
		new_osob2 := (temp shr 16);
		add_tail(tail, new_osob1);
		add_tail(tail, new_osob2);
	end;
	2: begin
		temp := crossing_two( o1, o2 );
		new_osob1 := (temp and max_word);
		new_osob2 := (temp shr 16);
		add_tail(tail, new_osob1);
		add_tail(tail, new_osob2);
	end;
	3: begin
		temp := crossing_universal( o1, o2,
								round(universal_chance * 100) );
		new_osob1 := (temp and max_word);
		new_osob2 := (temp shr 16);
		add_tail(tail, new_osob1);
		add_tail(tail, new_osob2);
	end;
	4: begin
		new_osob1 := crossing_odnor( o1, o2 );
		add_tail(tail, new_osob1);
	end;
	end;
end;

procedure mutation( o: word );
begin
	case mutate_mode of
	1: begin
		new_osob1 := mutation_change( o, round(change_chance * 100) );
		add_tail(tail, new_osob1);
	end;
	2: begin
		new_osob1 := mutation_swap( o );
	add_tail(tail, new_osob1);
	end;
	3: begin
		new_osob1 := mutation_reverse( o );
	add_tail(tail, new_osob1);
	end;
	end;
end;

begin
	Write('Войти в тестовый режим? (1/0) : ');
	{$I-} flag := false;
	while ( not flag ) and ( not eof ) do
	begin
		readln(test);
		if ( IoResult = 0 ) and ( test in [0, 1] ) then
			flag := true;
		if not flag then
			write('Неверный ввод, повторите: ');
	end;
	if not flag then
	begin
		writeln('Вы ничего не ввели!');
		halt(1);
	end; {$I+}
	writeln();

	{чтение из файла}
	assign( f, 'config.txt' );
	reset( f );
	while not eof( f ) do
	begin
		while not eoln( f ) do
		begin
			repeat
				Read(f, c);
				if c = '#' then goto next_line;
			until c <> ' ';

			Read(f, s);
			s1 := copy( s, 0, pos(' ', s) - 1 );
			s := copy( s, pos(' ', s) + 3, 10 );
			s := copy( s, 0, pos(' ', s) - 1 );
			val (s, znach);
			{if not prisvaivanie(s1, znach) then
				halt(1);}
			prisvaivanie(s1, znach);

			next_line: break;
		end;
		Readln(f);
	end;

	{контроль ввода}
	if ( pop_volume <= 5 ) or ( preserv_high < 1 ) or
	( preserv_high > pop_volume / 3 ) or ( preserv_low < 1 ) or
	( preserv_low > pop_volume / 3 ) or ( kill_proporc < 1 ) or
	( kill_proporc > pop_volume * 2 / 3 ) or
	( not select_mode in [1, 2] ) or
	( not cross_mode in [1, 2, 3, 4] ) or
	( not mutate_mode in [1, 2, 3] ) or ( max_valueless_iters < 2 ) or
	( max_iter < 2 ) or ( porog_usech < 0.1 ) or
	( porog_usech > 0.9 ) or ( universal_chance < 0.1 ) or
	( universal_chance > 0.9 ) or ( change_chance < 0.1 ) or
	( change_chance > 0.9 ) or ( quality_epsilon < 0 ) or
	( variability < 0.1 ) or ( variability > 0.9 ) or
	( crossing_volume < 0.1 ) or ( crossing_volume > 0.9 ) then
	begin
		Writeln('Wrong config file!');
		halt(1);
	end;
	close( f );

	if test = 1 then 
		Writeln('Журнал записан в файл')
	else Writeln('Результат записан в файл');

	assign( output, 'output.txt' );
	rewrite(output);

	init( head, tail, pop_volume );
	povtor := 0;
	prev_q := tail^.quality;

	k := 0;
	start := GetTickCount64;
	{цикл алгоритма}
	while k <= max_iter do
	begin
		k := k + 1;
		writeln('-----------------ITERATION ', k, '------------------');
		sort_vst( head, tail );
		if test = 1 then
		begin
			writeln('----------------------sort----------------------');
			print( head );
		end
		else begin
			write('Лучшая точка: '); print_bin( tail^.osob );
			write(' = ', osob_to_x( tail^.osob ) : 1 : 6);
			Writeln('   Значение в ней: ', tail^.quality : 1 : 6);
		end;
		if abs( tail^.quality - prev_q ) < quality_epsilon then
			povtor := povtor + 1;
		if povtor = max_valueless_iters then
		begin
			Writeln();
			writeln('Достигнуто макс кол-во итераций без улучшения');
			break;
		end;
		if abs( tail^.quality - enough_value ) < quality_epsilon then
		begin
			Writeln();
			Writeln('Достигнуто предельное значение');
			break;
		end;

		prev_q := tail^.quality;
		selection();
		if test = 1 then
		begin
			writeln('----------------------select--------------------');
			print( head );
		end;
		{скрещивание}
		num := round( num_of_all( head ) * crossing_volume / 2 );
		for i := 1 to num do
		begin
			shot1 := random( num_of_all( head ) ) + 1;
			repeat
				shot2 := random( num_of_all( head ) ) + 1;
			until shot1 <> shot2;
			x := head;
			for j := 1 to ( shot1 - 1 ) do
				x := x^.next;
			osob1 := x^.osob;
			x := head;
			for j := 1 to ( shot2 - 1 ) do
				x := x^.next;
			osob2 := x^.osob;
			crossing( osob1, osob2 );
			if num_of_all( head ) > ( pop_volume - 1 ) then break;
		end;
		if test = 1 then
		begin
			writeln('----------------------cross---------------------');
			print( head );
		end;
		selection();
		if test = 1 then
		begin
			writeln('----------------------select--------------------');
			print( head );
		end;
		{мутация}
		num := round( num_of_all( head ) * variability / 2 );
		for i := 1 to num do
		begin
			shot1 := random( num_of_all( head ) ) + 1;
			x := head;
			for j := 1 to ( shot1 - 1 ) do
				x := x^.next;
			osob1 := x^.osob;
			mutation( osob1 );
			if num_of_all( head ) > pop_volume then break;
		end;
		if test = 1 then
		begin
			writeln('-----------------------mutate-------------------');
			print( head );
		end;
		{убирание дубликатов и дозаполнение}
		duplicate( head, tail );
		fill( head, tail, pop_volume );
		if test = 1 then
		begin
			writeln('----------------duplicate & fill----------------');
			print( head );
		end;
	end;
	finish := GetTickCount64;
	if k = max_iter then
	begin
		Writeln();
		Writeln('Достигнути макс кол-во итераций');
	end;
	Writeln('----------------------------------------------');
	write('Конечная точка: '); print_bin( tail^.osob );
	write(' = ', osob_to_x( tail^.osob ) : 1 : 6);
	Writeln('   Значение в ней: ', tail^.quality : 1 : 6);
	writeln('Число итераций: ', k);
	Writeln('----------------------------------------------');
	WriteLn('Spent ', (finish - start), ' milliseconds' );

	destruct( head, tail );
end.
