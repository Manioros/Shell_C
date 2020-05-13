Maniwros Iwannis 
===========================
O phgaio kwdikas tou mysh apoteleitai apo 3 arxeia:
* main.c: to kyrio meros tou programmatos 
* cmd_table.h: header file tou cmd_table.c arxeiou
* cmd_table.c: synarthseis gia xeirismo twn entolwn (cmd_item) sth domh pinaka

Oi entoles apokwdikopoiountai apo thn grammh eisodou me ton akoloutho format:
arg0 arg1 arg2 | arg0 arg1 arg2 | ... */

Oi entoles diaxwrizontai metaksy tou me to symvolo pipe (|). Kathe entolh apoteleitai apo arguments me to prwto argument na einai to onoma tou pros ektelesh programmatos. Meta ta arguments mporei na akolouthoun ta symvola >, >>, < kai &.

To kyrio meros tou programmatos akolouthei peripou to diagramma pou syzhththike sto frontisthrio:
* emfanish prompt
* anagnwsh grammhs eisodou, read_line()
* apokwdikopoihsh ths grammhs eisodou (parse_line()) kai kataxwrish ston pinaka cmd_table
* epanalhptikh ektelesh olwn twn entolwn tou cmd_table
	* elegxos kai ektelesh exit kai fg
	* ektelesh entolhs xrhsh, p = get_cmd(i);
		* parent
			* fylaksh prohgoumenou pipe
			* xeirismos ekteleshs sto bg
		* child
			* fork
			* kathorismos anakatefthinshs eisodou kai eksodou
			* xeirismos twn pipe file descriptors
			* exec

Sth main.c ginetai kai o xrirismos twn ekshs shmatwn:
* SIGINT    : Sto ^C to paidi diakoptei th leitourgia tou
* SIGTST    : Sto ^Z den ginetai kapoios xeirismos, to paidi lamvanei to shma kai ginetai paused
* SIGCHLD   : O pateras perimenei to paidi na allaksei state. H alalgh state mporei na afora paush me ^Z h' termatismo.

Me thn entolh fg, stelnetai shma SIGCONT apo ton patera sta paidia wsta na synexisoun th leitourgia tous. O pateras lamvanei to shma SIGCHLD kai perimenei mexri th nea allagh state.

Ti den exei ylopihtheiQ
* H fg epanaferei ola ta paidia pou einai se katastash pause kai oxi mono ena
* To TAB den symplhrwnei automata to onoma twn arxeiwn





