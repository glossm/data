form Get arguments
    sentence i_file
    sentence o_dir
    sentence o_prefix
    sentence o_type
    sentence t_file_full
endform

data = Open long sound file: i_file$

tagtable = Read Table from tab-separated file: t_file_full$

select tagtable
rows = Get number of rows

for i from 1 to rows
	start = object [tagtable, i, 1]
	end = object [tagtable, i, 2]
	select data
	partdata = Extract part: start, end, 0
	select partdata

	if i <= 9
       numbering$ = "000" + fixed$ (i,0)
    elsif i <= 99
       numbering$ = "00" + fixed$ (i,0)
    elsif i <= 999
       numbering$ = "0" + fixed$ (i,0)
    else
       numbering$ = fixed$ (i,0)
    endif

	Rename: fixed$ (i,0)
	Save as WAV file: o_dir$ + "/" + o_prefix$ + "_" + numbering$ + o_type$
	removeObject: partdata
endfor

removeObject: data
removeObject: tagtable
