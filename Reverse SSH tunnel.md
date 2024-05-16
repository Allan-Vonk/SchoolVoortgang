# Introductie

In basis termen gaan we een connectie maken naar een beveiligd netwerk via een reverse tunnel. Eerst gaan we vanuit het netwerk naar een publieke server "Tunnelen". Sommige netwerken blokkeren outbound ssh connecties, dit zou betekenen dat we geen reverse ssh tunnel kunnen aanleggen. Als je je eigen netwerk opzet zou je hier rekening mee kunnen houden.

In deze documentatie gaan we de volgende dingen behandelen

1. [[#Outbound SSH tunnel]]
2. [[#Reverse Entry]]
3. [[#More port forwarding]]
4. [[#Listening interface]]
5. [[#Services]]
6. [[#SSH Flags]]

# Outbound SSH tunnel

Om te beginnen hebben we 1 linux server nodig die we vanaf nu de proxy server noemen, zorg ervoor dat deze server al toegankelijk is met ssh. In Azure moet je dan ook nog via de firewall settings de poort open zetten voor inbound traffic. Als je het op je eigen netwerk host moet je het via de instelling van de router regelen. Standaard SSH port is 23

Het is belangrijk dat je er voor zorgt dat de ssh authentication met ssh keys gaat, dit voorkomt een hoop gezeik als we het gaan automatiseren met Services. Hier kun je meer lezen over hoe dit werkt [SSH keys](https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server).

Vervolgens kunnen we de initiele ssh connectie opstarten
```bash
ssh -i <SSh key location> -R <Listen port>:localhost:22 <naam>@<Server adress>
```
De -i betekent dat we een permission key willen selecteren, dit kan staan waar je het zelf wilt, maar ik adviseer om het in de ~/.ssh/ folder te zetten. Als je de file goed neerzet met de naam id_rsa dan kun je theoretisch dit deel weglaten, maar voor de zekerheid zou ik toch specificeren welke file je wilt gebruiken. 

De -R flag geeft aan dat de connecties op de aangegeven TCP port of Unix socket op de proxy server word geforward naar de locale server (Raspberry pi).
De format is als volgt:
```
-R <Listen port>:<Host>:<Send port>
```
Waar de listen port op de proxy server word afgeluisterd en word doorgestuurd naar de send port, zorg er dus voor dat de Listen port vrij is voor gebruik. Iets als 9000.

En dan hebben we natuurlijk op het einde de rest van de ssh syntax
```
<naam>@<Server adress>
```
dit geeft aan als welke gebruiker we willen inloggen en op welk server adres. zorg ervoor dat dit overeenkomt met de user waar je de SSH key voor hebt gegenereerd.

# Reverse Entry

Het volgende gaan we op de proxyserver doen.

Nu dat we een connectie hebben van de raspberry pi en de proxy server kunnen we ook via de proxy server naar de raspberry pi connecten.
```bash
ssh -p <Listen port> <naam>@<adress>
```
De -p geeft aan dat we op een aangepaste port willen verbinden met de server. Voor het volgende voorbeeld gebruiken we port 9000 die we eerder hebben gebruikt.

```bash
ssh -p 9000 admin@localhost
```
Omdat we dus eerder de ssh connectie van de raspberry pi naar de proxy server hebben gemaakt die luisterd op port 9000 kunnen we op deze manier met de raspberry pi verbinden op de localhost. hier is het minder belangrijk of dat de raspberry pi ssh keys gebruikt of niet, we gaan er hier van uit dat dit of niet het geval is of dat de ssh keys zo staan ingesteld in de correcte folder dat de ssh service er vanzelf bij kan. 

Eigenlijk hebben we nu al het basis concept van de reverse ssh tunnel behandeld. We zijn verbonden met de raspberrypi via de proxy server. Om dit te testen zou je even op een apart netwerk de verbinding moeten uit proberen.

# More port forwarding

Soms wil je naast de ssh port ook andere ports open zetten, voor bijvoorbeeld postgresql, flask of node-red. Gelukkig kunnen we ook dit vrij simpel via onze ssh tunnel routen. Eerder hebben we al de ssh port geforward naar als voorbeeld port 9000 op de proxy server met het volgende commando:
```bash
ssh -i <SSh key location> -R <Listen port>:localhost:22 <naam>@<Server adress>
```
Met de -R flag hebben we de listen port (9000) door verbonden met port 22 op onze locale server. als je simpelweg meer -R flags gebruikt kun je ook andere ports doorverbinden.
```bash
ssh -i <SSh key location> -R 9000:localhost:22 -R 5432:localhost:5432 -R 1880:localhost:1880 <naam>@<Server adress>
```
Zorg er ook in dit geval wel weer voor dat de ports vrij zijn. port 22 kunnen we bijvoorbeeld niet gebruiken omdat de proxy server zelf ook bereikbaar moet zijn op ssh.

Op deze manier hebben we onze services bereikbaar gemaakt vanuit de proxy server.

# Listening interface

Nu moeten we eerst zelf verbinden met de proxy server, dit word een beetje vervelend als we services zoals node-red en postgresql willen gebruiken, deze kunnen niet altijd via een ssh tunnel worden verbonden. Dit komt doordat de -R flag standaard op localhost forward.

Als je zou kijken met een port commando:
```bash
sudo netstat -tulpn | grep LISTEN
```
*zorg ervoor dat je netstat hebt geinstalleerd om dit te zien*

Dan zul je zien dat er word geluisterd op de adressen 127.0.0.1:"port"  . Echter kunnen we hier alleen locaal bij. Om dit van buiten af toegankelijk te maken willen we dat er staat 0.0.0.0:"port" . Er staat er hier al 1 van, dit is de SSH van de proxy server.

Om dit te kunnen maken moeten we de syntax van de outbound ssh code een klein beetje aanpassen. Momenteel hebben we het volgende.

```bash
ssh -i <SSh key location> -R <Listen port>:localhost:5432 <naam>@<Server adress>
```
We moeten een kleine aanpassing maken aan onze -R flag

```bash
-R <Interface>:<Listen port>:localhost:5432
```
Zoals je kunt zien hebben we een interface gespecificeerd, dit verteld uiteraard op welke interface we naar de port moeten luisteren, standaard was dit 127.0.0.1, dit is de localhost. Hier willen we 0.0.0.0 van maken.

Hier is een voorbeeld
```bash 
-R 0.0.0.0:5432:localhost:5432 admin@<server adress>
```

Zou het niet mooi zijn als we het zo al hebben gefixt :). Dit is echter niet het geval. We moeten eerst wat aanpassingen maken in de config files van ssh. Dit moeten we doen op de raspberry pi.

```bash
sudo nano /etc/ssh/sshd_config
```
Gebruik dit commando om de sshd_config aan te passen. scroll vervolgens naar beneden totdat je de optie voor GatewayPorts ziet staan. deze is gecomment met een # en staat standaard op 'No'. Verander deze line naar "GatewayPorts yes".

Save vervolgens de file door Ctrl + O in te toetsen en vervolgens enter om hem in dezelfde file te schrijven. Ctrl + X om uit nano te gaan.

# Services

Soms wil je dat een commando automatisch opstart als je de pi aansluit, of dat als het verbreekt zoals bij een internet storing het zichzelf herstelt. Dit kun je op verschillende manieren uitvoeren, voor dit voorbeeld gebruiken we services. Met services kun je makkelijk scripts en commando's in een gecontroleerde omgeving gebruiken, dit bied de mogelijkheid om zichzelf te herstellen en op te starten.


Om een service te maken moet je eerst een file aanmaken met het .service file type:
``` bash
sudo touch /etc/systemd/system/reverse-ssh.service
```
Het is belangrijk dat het in de correcte folder staat en een .service file is. Anders kan systemctl hem niet vinden.


Vervolgens kunnen we deze file aanpassen met Nano of een andere file editor:
``` bash
sudo nano /etc/systemd/system/reverse-ssh.service
```

Daarna kunnen we de service gaan aanmaken. Om meer te leren over services kun je deze informatie doornemen: [Systemctl](https://www.digitalocean.com/community/tutorials/how-to-use-systemctl-to-manage-systemd-services-and-units)
```
[Unit] 
Description=SSH reconnect 
After=network.target 

[Service]
Type=simple 
Restart=always 
RestartSec=<IntervalRestart> 
User=<user> 
ExecStart= /bin/ssh -i <SSh key location> -R 9000:localhost:22 -R 5432:localhost:5432 -R 1880:localhost:1880 <naam>@<Server adress>

[Install] WantedBy=multi-user.target
```

Nadat we de file hebben gemaakt moeten we systemctl herladen zodat hij opnieuw de files kan vinden.
``` bash
sudo systemctl daemon-reload
```

Vervolgens kunnen we de service starten en 'aanzetten' zodat hij ook op de startup aan gaat.
```bash
sudo systemctl start reverse-ssh 

sudo systemctl enable reverse-ssh
```

Om te kijken of het werkt kunnen we de status van de service checken met het volgende commando:
```bash
sudo systemctl status reverse-ssh
```


# SSH Flags

Het lijkt nu of alles werkt, dit is echter nog niet het geval. We moeten nog een paar SSH flags toepassen in ons commando. Om meer te leren over ssh flags kun je hier meer leren: [SSH Flags](https://linuxcommand.org/lc3_man_pages/ssh1.html)

Voor onze toepassing moeten we er voor zorgen dat ssh niet opent in een interactive mode, dit zorgt er namelijk voor dat het gelijk weer afsluit. Doordat er geen terminal is waar de gebruiker dingen kan invoeren sluit het automatisch de service af. Gelukkig zijn hier wat nuttige flags voor. 

De -N en -n flags zorgen ervoor dat de ssh als 'non-interactive' word gestart. 

Dat ziet er zo uit in onze commando:
``` bash
/bin/ssh -i <SSh key location> -N -n -R 9000:localhost:22 -R 5432:localhost:5432 -R 1880:localhost:1880 <naam>@<Server adress>
```

Vergeet dit niet aan te passen in de service file.