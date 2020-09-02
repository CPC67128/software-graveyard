# PiSomfyRemoteControl
Raspberry Pi / Somfy Remote Control

## Installation du Raspberry Pi

Setup RASPBIAN JESSIE LITE (Minimal image based on Debian Jessie)
sudo raspi-config
> expand filesystem
> enable Raspberry Camera
> enable SSH
> overclock to Modest :-)
> GPU memory set to 64 instead of 128

L'adresse IP de mon Raspberry Pi est 192.168.0.10

## Installation d'Apache 2

sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade

sudo apt-get install apache2

/* Tester http://192.168.0.10/ et vérifier que la page de test d'Apache s'affiche */

## Installation de PHP 

sudo apt-get install php5
php -v

/*
PHP 5.6.14-0+deb8u1 (cli) (built: Oct 28 2015 00:02:05)
Copyright (c) 1997-2015 The PHP Group
Zend Engine v2.6.0, Copyright (c) 1998-2015 Zend Technologies
    with Zend OPcache v7.0.6-dev, Copyright (c) 1999-2015, by Zend Technologies
*/

## Installation de MySQL

sudo apt-get install mysql-server

sudo apt-get install mysql-client
mysql -u root -p

/* Test */

sudo apt-get install php5-mysql

## Installation de phpMyAdmin

cd /var/www

sudo wget https://files.phpmyadmin.net/phpMyAdmin/4.5.2/phpMyAdmin-4.5.2-all-languages.tar.gz
sudo tar -xzvf phpMyAdmin-4.5.2-all-languages.tar.gz
sudo mv phpMyAdmin-4.5.2-all-languages phpMyAdmin
sudo rm phpMyAdmin-4.5.2-all-languages.tar.gz

sudo vi /etc/apache2/sites-enabled/000-default.conf
/*
Change DocumentRoot /var/www/html to DocumentRoot /var/www/
*/
sudo service apache2 restart

/* Test http://192.168.0.10/phpMyAdmin/ */

## Installation de proFTPd

sudo apt-get install proftpd
/* Run proftpd : standalone */

cd /var
sudo chmod 777 www

## Installation des outils GPIO de Gordon @ Drogon
 
sudo apt-get install git-core
cd
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build

gpio -v
gpio readall

Information: http://wiringpi.com/

If downloaded:
tar xfz wiringPi-98bcb20.tar.gz
cd wiringPi-98bcb20
./build

## Installation de l'application "Commande Volets Somfy"

Se connecter à phpMyAdmin et créer la base de données "Raspberry" en utilsant le script "Database.sql"

Ouvrir un client FTP (FileZilla par exemple) et se connecter au Pi avec le compte pi
Transférer la répertoire SomfyRemoteControl vers /var/www
Configurer le fichier SomfyRemoteControl/configuration.php avec les identifiants de connection MySQL (J'utilise root... je sais c'est mal)

Tester : http://192.168.0.10/SomfyRemoteControl/