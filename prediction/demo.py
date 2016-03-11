#coding: utf-8
from embed import Register

with Register() as register:
    #Nous crééons une radio, qui va ici nous diffuser différents genres de musique.
    techno_counter = 0
    house_counter = 0
    sample_radio = register.add_radio()
    print "Nouvelle radio ! Id : %d" % (sample_radio)
    #Nous avons ici la liste des genres répertoriés par la radio. undefined correspond au noeud de départ.
    genre_list = ["house", "techno"]
    #On ajoute nos genres dans le radio créée
    id_house = 0
    id_techno = register.add_content(sample_radio) #ID:1 techno
    print "Demandons 20 morceaux."
    for i in range(1, 20):
        a = genre_list[register.get_next_content(0)]
        print(a)
        if a == "house":
            house_counter += 1
        elif a == "techno":
            techno_counter += 1
    print "Nombre de tracks techno : %d, Nombre de tracks house : %d" % (techno_counter, house_counter)
    print "Nous adorons la techno. Demandons encore 10 morceaux, cette fois en indiquant que la techno nous plait."
    raw_input("Appuyez sur enter pour demander 10 morceaux")
    techno_counter = house_counter = 0
    for i in range(1, 20):
        a = genre_list[register.get_next_content(0)]
        print(a)
        if a == "house":
            house_counter += 1
        elif a == "techno":
            register.apply_feedback(sample_radio, 1)
            techno_counter += 1
    print "Nombre de tracks techno : %d, Nombre de tracks house : %d" % (techno_counter, house_counter)
    print "Fin de la demo !"
    # register.add_content(0)
    # genre_list.append("house")
    # for i in range(1, 20):
    #     a = genre_list[register.get_next_content(0)]
    #     print(a)
    #     if a == "house":
    #         print("applying feedback")
    #         register.apply_feedback(0, 1)
