/*
	+==========================================+
	|	 		IMPORTANT /!\                  |
	+==========================================+

	Pour une meilleure compréhension, les conclusions sont à lire après avoir lu
	le reste des commentaires qui composent le bloc de code.
*/

int32_t main(int32_t argc, char** argv, char** envp)
{
	while (true)
	{
		uint32_t* const fp; 
		printf("%p, %p \n", auth, service); //Affiche l'addresse de auth et service (qui sont des global)
		bool c_1 = false;
		char buf;
		bool z_1 = !fgets(&buf, 0x80, stdin); //Récupere notre input

		if (z_1) //On break si fgets ne récupere pas de char
			break;

		int32_t i = 5; //Notre count
		char* esi_1 = &buf; //L'entré utilisateur
		char const* const edi_1 = "auth "; //Une string "auth"

		while (i) //Conclusion : on check que notre entry user est égal a "auth "
		{
			char temp0_1 = *esi_1; //Premier char de l'entré user
			char const temp1_1 = *edi_1; //Premier char de la string "auth"
			c_1 = temp0_1 < temp1_1; //Si premier char user entry est inferieur a premier char string alors c'est true
			z_1 = temp0_1 == temp1_1; //Si premier char user entry est égal a premier char string alors c'est true
			esi_1 = &esi_1[1]; //On increment l'entré user
			edi_1 = &edi_1[1]; //On increment la string "auth"
			i -= 1; // on décrement notre count

			if (!z_1) //Si les chars ne sont pas égaux on break cette boucle
				break;
		}
		bool c_2 = false;
		bool z_2 = !((!z_1 && !c_1) - c_1); //z_1 sorti de boucle == true
						    //c_1 sorti de boucle == false
						    //(false) - false == false
						    //Donc z_2 = true (y a un point d'exclamation), si l'user entru est "auth"

		if (z_2) // /!\ On veut rentrer la dedans
		{
			auth = malloc(4); //Malloc de 32 bits
			**&auth = 0; //On set le premier char de auth par \0 
			int32_t i_1 = 0xffffffff; //i_1 est égal a -1 car on il est overflow
			char var_8b;
			char* edi_2 = &var_8b; //On range l'addresse de var_8b

			while (i_1) //Boucle useless on peut l'ignorer rien ne se passe dedans
			{
				bool cond:4_1 = 0 != *edi_2;e
					edi_2 = &edi_2[1];
				i_1 -= 1;

				if (!cond:4_1)
					break;
			}

			c_2 = ~i_1 - 1 < 0x1e; //Comparaison avec de la magie noir, en gros c_2 == true
			z_2 = ~i_1 == 0x1f; //False

			if (z_2 || c_2) //c_2 est true si l'entry user est "auth "
				strcpy(auth, &var_8b); //On remplace l'addresse de auth par l'addresse de var_8b (on perd notre malloc)
		}

		int32_t i_2 = 5;
		char* esi_2 = &buf;
		char const* const edi_3 = "reset";

		while (i_2) //Conclusion : on check que notre entry user est égal a "reset"
		{
			char temp2_1 = *esi_2;
			char const temp3_1 = *edi_3;
			c_2 = temp2_1 < temp3_1;
			z_2 = temp2_1 == temp3_1;
			esi_2 = &esi_2[1];
			edi_3 = &edi_3[1];
			i_2 -= 1;

			if (!z_2)
				break;
		}

		bool c_3 = false;
		bool z_3 = !((!z_2 && !c_2) - c_2); //z_3 == true si l'user entry est "reset"

		if (z_3) //Si c'est true on free auth /!\ On veut pas entrer la dedans
			free(auth);

		int32_t i_3 = 6;
		char* esi_3 = &buf;
		char const* const edi_4 = "service";

		while (i_3) //Conclusion : on check que notre entry user est égal a "service"
		{
			char temp4_1 = *esi_3;
			char const temp5_1 = *edi_4;
			c_3 = temp4_1 < temp5_1;
			z_3 = temp4_1 == temp5_1;
			esi_3 = &esi_3[1];
			edi_4 = &edi_4[1];
			i_3 -= 1;

			if (!z_3)
				break;
		}

		bool c_4 = false;
		bool z_4 = !((!z_3 && !c_3) - c_3); //z_4 == true si l'user entry est "service"

		if (z_4)
		{
			c_4 = &buf >= 0xfffffff9; //Inutile
			z_4 = &buf == 0xfffffff9; //Inutile
			char s[0x79];
			service = strdup(&s); //On alloc service (ce qui explique) qu'a chaque fois sa zone memoire est plus haute
		}

		int32_t i_4 = 5;
		char* esi_4 = &buf;
		char const* const edi_5 = "login";

		while (i_4)//Conclusion : on check que notre entry user est égal a "login"
		{
			char temp7_1 = *esi_4;
			char const temp8_1 = *edi_5;
			c_4 = temp7_1 < temp8_1;
			z_4 = temp7_1 == temp8_1;
			esi_4 = &esi_4[1];
			edi_5 = &edi_5[1];
			i_4 -= 1;

			if (!z_4)
				break;
		}

		if (!((!z_4 && !c_4) - c_4)) //True si l´entry user est "login"
		{
			if (!*(auth + 0x20)) //Si auth[8] == \0 on rentre la dedans
			{
				fp = stdout;
				fwrite("Password:\n", 1, 0xa, fp);
			}
			else //On souhaite entrer ici pour l'exploit
				system("/bin/sh");
		}
	}
}
