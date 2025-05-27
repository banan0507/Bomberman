https://uptro29158-my.sharepoint.com/:w:/g/personal/alexandru_sicoie_student_upt_ro/EeSxWyYkLwVHqidepdSvj0oB0mzJ15APSmSv4dgjGwX61Q?e=bNAb4Z

BOMBERMAN – SPECIFICATII 

Sicoie Alexandru 

 

Github-ul meu: aici 

 

Bomberman – versiune terminal: 

-Pt versiunea de terminal am facut un program simplu care printeaza playerul ca fiind P, bomba cu B si peretii cu # si @; 

-folosim o matrice pt a printa tot jocul; 

-playerul are ca spawn locatia 11 din matrice care este prima pozitie; 

-trebuie sa avem o matrice cu n impar pt a se pune peretii random frumos pe harta; 

-miscarea pe harta se face cu WASD,  si ca si input putem primi wasd sau wasd, cu inputul b/B punem bomba care are un delay (in secunde dat de noi); 

-daca inputul nu corespunde celor de mai sus va printa pe ecran un mesaj care sa ne ghideze; 

-dupa fiecare iteratie folosim o functie sa stergem ce e pe ecran; 

-peretii indestructibili sunt cei cu # care sunt pe margini, si in mijlocul matricei cu spatiu de cate 1 intre ei, iar cei destructibili sunt cei cu @; 

-scopul este sa distrugem peretii @ care au spawn random, jocul nu are un ending; 

 

Bomberman – versiune 2D: 

-Jocul contine o harta facuta dintr o matrice in care pun 0, 1 sau 2, depinde la ce tile ne referim, 0 e cel solid, adica marginea si cei din mijloc, 1 e iarba pe care se poate plimba playerul si 2 este peretele care se spawneaza random; 

-Toate dimensiunile jocului, rezolutie, marime harta s.a. sunt controlate cu define uri pt a fi modificate usor; 

-Am folosit SDL3 pt partea grafica, cu care desenez playerul, ai urile si harta in sine, toate acestea le extrag dintr un spritesheet; 

-Jocul are un ecran pt inceperea jocului, unul pt Lose Screen, la care se ajunge daca playerul se sinucide cu bomba sau daca un Ai intra in el, si Win Screen la care se ajunge daca toate Ai-urile au fost omorate, jocul are si functia de retry care ia jocul de la inceput sau de quit; 

-O data ce incepe jocul tile urile random se iau cu o sansa de 25% de spawn, iar playerul este pe pozitia 11 din matrice, cu specificatia ca nu se pot spawna tile uri sau ai in player sau langa el, pt a avea spatiu, iar ai ul se spawneaza random pe harta; 

-Playerul se misca cu WASD si bomba se plaseaza cu B,  cu 2 sec delay pana explodeaza, are range de 1 pe fiecare parte, bomba se pune pe pozitia playerului, dar cand playerul iese de pe acel tile are coliziune cu bomba, adica ca sa nu intre din nou pe acel tile, scopul fiind sa omoram toate ai urile cu bomba; 

-Playerul are animatie in functie de directia in care merge, adica se schimba poza pe care o afisam, la fel si la Ai; 

-Jocul are si o iconita cu clasicul bomberman;	Picture 

Ecrane win/lose/begin:  

PicturePicturePicture 

 

Functii interesante si metode folosite: 

PictureFunctia asta este folosita pt a pune tile urile random pe harta (cele care pot fi distruse, cu mentiunea ca tile 0 este cel solid, 1 iarba, si 2 cel random), folosim rand cu o sansa de 25% de spawn si verifica sa fie tile ul potrivit si sa nu fie pe spawn-ul playerului si nici pe cel de spawn ai. 

 

 

Functia urmatoare este folosita pt desenarea playerului, care daca primeste W, A, S sau D schimba imaginea pe care o afiseaza pt ca miscarea sa fie cat mai smooth se poate intr un mediu 2D. Noi luam coordonatele dintr un spritesheet si la fiecare trebuie specificata si dimensiunea, si am mai multe variabile globale in functie de care se alege pozitia. Foarte similar arata si functiile de afisare mapa si afisare AI. 

PicturePicture 

Aici avem o portiune din map.h, din fisierul header, unde folosesc #pragma once care inlocuieste classicul ifndef si endif; includem bibliotecile grafice, iar in celelalte headere includem map.h; si cel mai important la define avem width si height, acestea reprezinta rezolutia care poate fi modificata foarte usor pt orice device, define ul ne ajuta sa modificam rapid in toate functiile unde este folosit. 

 
