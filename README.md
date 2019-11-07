# Pharos
Your financial buddy

Ideeën:

Access
	Form maken voor dataentry die 1 tabel voedt
	Report maken op basis van die tabel
	Evalueren hoe data entry daar gaat
	Mogelijks goede start om na te denken over DB structuur
		Meh, don't sweat the little things… In dev blog voor wow kon je ook zien dat classic wow helemaal geen perfecte DB normalisatie had. Beter simpel beginnen met enkele tabellen en dan zien we wel. 
	
SQL server
	Integratie met excel? Dit verbetert wel niet het belangrijkste punten voor mij (meer automatisch data entry), het opslaan van data en rapporten daarop zijn minder een probleem, dat kan in principe in excel blijven.
	
	Om data entry te automatiseren zal ik wel programma zelf moeten maken. Dan is eigenlijk wel logisch om die data naar een DB weg te schrijven en niet naar een excel file. Mogelijks is een XML file wel een goed en makkelijker alternatief. 
	
Belgische fintech
	CODA is een specifiek formaat dat wordt gebruikt voor veel boekhoudingspakketten
	
	Europese richtlijn PSD2 -> dit verplicht banken feitelijk om data van hun accounts beschikbaar te maken voor fintech apps mits nodige beveiliging en toestemming van de klant.
	
	Isabel Group is de grootste fintech speler in Belgie. Producten:
		Zoomit voor particulieren
		Isabel 6: online betalingsoplossing voor bedrijven. Dient eigenlijk om betalingen te regelen over banken heen (tot 25) en om te integregeren met boekhoud software of ERP.
		
		Ook functies voor fintech apps
		
		https://www.isabelgroup.eu/nl/oplossingen/ibanity/
			>> 1 centrale API om info te halen van alle banken -> kan goede eerste link zijn
			
	Toshl lijkt grootste bestaande speler maar heeft ook vooral connecties met de US en nog niets in België (al kent niemand ervan in België natuurlijk - je zou het al heel actief moeten opzoeken + auto bank transfer is enkel optie in duurste abonnement)
	
Idee voor Finances 0.1
	Download to CSV vanuit Keytrade (want de XML file klopt niet en CSV valt ook makkelijk gestructureerd in te lezen in C++). 
	Nodige manipulaties en regels hardcoden in het C++ programma
	Output in XML file
	XML file kan dan ingelezen worden in Excel -> de xml file is wel exact de format die excel nodig heeft, dan kan je in excel nog altijd de nodige aanpassingen doorvoeren indien nodig (en zo iteratief verbeteren)
	De historische database en de visualisatie blijft in dit geval nog in Excel.
		Historische database hoeft geen excel te zijn! Je kan het programma laten wegschrijven naar één centrale XML file en daar steeds entries toevoegen. Via power query kan je die XML inladen in een excel table en blijft die connectie ook behouden. Je hoeft dan enkel op refresh te duwen en dan komt de nieuwe data erin. 
	
	Om weg te schrijven van en XML file opnieuw in te lezen >> tiny XML 2. Simpelweg één h en cpp file include en lijkt al het nodige te bevatten. 
	
	Host op Github? Version control zo basic mogelijk laten in dit stadium
	
Finances 0.2
	Opzetten van een database link -> naar mijn gevoel het belangrijkste om te doen voor de gui. Dan zit de data backend toch al meer in orde.
	
Finances 0.3
	Eerste GUI maken
		Ofwel een windows desktop app -> via Qt
Ofwel onmiddellijk proberen om een webapp te maken -> via Django -> hier wel Python leren (maar dat is sowieso goed! Besides, how hard can it be…)
