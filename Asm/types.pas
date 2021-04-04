unit types;

interface
const
	m = 16;
	max_word = 65535;
type
	ref_list = ^list;
	list = record
		osob: word;
		quality: real;
		chance: integer;
		next: ref_list;
		prev: ref_list;
	end;
	
implementation

begin
end.
