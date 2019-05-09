# การทำ WORDPRESS บน Virtual Machine (Ubuntu)
## อันนี้จะไม่มีการใช้ Proxy ของ Mahidol หรือใช้ MU-WIFI 

    1. ตั้งค่า Network Adapter ก่อนเข้าใช้ Virtual Box โดยสามารถเลือกได้สองแบบ คือ
       1. NAT (แนะนำให้เลือกตัวนี้)
       2. Bridge Network Adapter
    2.  เปิด Virtual Box ที่ติดตั้ง Ubuntu ขึ้นมาใช้งาน
    3.  เปิด terminal ขึ้นมาเพื่อหา ip address ของเราโดยพิมพ์
```console
ifconfig
```
ถ้าขึ้นว่า **ifconfig command not found** ให้ใช้คำสั่งต่อไปนี้เพื่อติดตั้ง
```console
sudo apt install net-tools
```
จากนั้นเมื่อติดตั้งสำเร็จพิมพ์ 
```console
ifconfig
```
จะได้หน้าตาแบบนี้ขึ้นมา <br>
![abc](asdsdasd)

จากนั้นจะติดตั้ง SSH หรือไม่ก็ได้ โดยการติดตั้งจะใช้คำสั่ง
```console
sudo apt install openssh-server
```
SSH มีไว้ควบคุม Virtual Box ผ่าน Command line โดยใช้ port 22 ในการติดต่อและควบคุม
เมื่อติดตั้งเสร็จให้ติดตั้ง PuTTY บน windows และเปิดขึ้นมา เมื่อเปิดขึ้นมาจะเจอหน้าต่าง PuTTY ให้กรอก IP Address จาก Ubuntu ที่เราใช้ ifconfig เพื่อดูเลข IP ลงไป แล้วกด open เพื่อเชื่อมต่อ การเชื่อมต่อครั้งแรกจะมีการบันทึก ssh-key ไว้เพื่อการยืนตัวตนในภายหลัง
เมื่อเชื่อมต่อได้สำเร็จ เราจะสามารถคววบคุม Ubuntu ผ่าน PuTTY ได้แล้ว
ต่อไปจะเป็นการติดตั้ง เพื่อสร้างเซิร์ฟเวอร์
ใช้คำสั่ง
```console
sudo apt install apache2

sudo apt-get install python-software-properties 

ถ้าใช้คำสั่งนี้แล้วติดตั้งไม่ได้ให้ใช้นี้แทน

->> sudo apt-get install software-properties-common <<-

sudo add-apt-repository ppa:ondrej/php

sudo apt-get update

sudo apt install php7.3 php7.3-cli php7.3-common

php -v ใช้คำสั่งนี้เพื่อดูเวอร์ชัน php ที่เราติดตั้งลงไปบนเครื่อง

sudo systemctl restart apache2 คำสั่งนี้เป็นการรีสตาร์ทเซิร์ฟเวอร์

```

เมื่อติดตั้งครบทุกอย่างแล้วให้ลองใช้สร้างหน้า php ขึ้นมาเพื่อดูว่าแสดงผลได้ถูกต้องหรือไม่

```console
sudo nano /var/www/html/phpinfo.php

```
จะได้ nano ขึ้นมาบน terminal จากนั้นพิมพ์ตามนี้
```php
<?php
phpinfo();
?>
```
เมื่อพิมพ์เสร็จแล้วให้กด Ctrl + X เพื่อทำการเซฟ แล้วกด Y อีกรอบแล้ว Enter ซ้ำเพื่อทำการยืนยัน

จากนั้นนำ IP Address ที่ดูด้วย ifconfig ไปเปิดใน browser ของ Windows
```console
ตัวอย่าง 192.168.XXX.XXX/phpinfo.php
```
จะได้หน้า php ที่บอกข้อมูลในเครื่องที่ติดตั้งมาทั้งหมด

## การติดตั้ง MariaDB 

เริ่มติดตั้ง Maria DB โดยใช้คำสั่ง

```console
sudo aptinstall mariadb-server mariadb-client
```
จากนั้นให้ใช้คำสั่งต่อไปนี้เพื่อเริ่มการทำงานและเช็คสถานะของ MariaDB 
```console
sudo systemctl start mysql
sudo systemctl status mysql
```
เปิดใช้งาน MariaDB 
```console
sudo mysql_secure_installation
```
จะมีหน้าต่างขึ้นมาให้ทำตามนี้จนเสร็จ
```
Enter the current password for root (enter for none): <Enter>
Note: clickEnter in case of setting the root password for the first time
Set the root password  > y
Respond to the prompt “New password” and “Re-enter new password:”
by settingthe root password.
Note:Important! Don’t forgetyour root password.
Remove anonymous user > y
Disallow the root login remotely > y
Reload privilege tables now > y
```
จากนั้นติดตั้ง php อีกรอบ
```console
sudo apt install php7.3-mysql
```
แล้วทำการ restart เซิร์ฟเวอร์อีกรอบโดยใช้คำสั่ง
```console
sudo systemctl restart apache2
```

## การติดตั้ง WORDPRESS
ใช้คำสั่ง
```console
cd
```
เพื่อกลับมาที่ Home directory หรือ แฟ้มหลักของ user นั่นเอง
ดาวน์โหลดไฟล์ WORDPRESS เวอร์ชันล่าสุดโดย
```console
wget -c http://wordpress.org/latest.tar.gz
```
หรือถ้าโหลดไม่ได้กดลิงค์นี้
[Download](https://drive.google.com/open?id=1uLb0EdYwpfTNEaxokbLdaCZ_0EwpEOYM)

ไฟล์ตัวนี้จะอยู่ที่ home directory ของเรา
จากนั้นทำการคัดลอกไฟล์โดยใช้คำสั่ง 
```console
sudo cplatest.tar.gz  /var/www/html
```
ไฟล์ตัวนี้จะถูกคัดลอกไปที่ พาธ ***/var/www/html*** <br>
จากนั้นเปลี่ยนพาธบน terminal โดยไปที่ /var/www/html โดยใช้คำสั่ง
```console
cd /var/www/html
```
ทำการคลายไฟล์ หรือ extract zip ที่เราโหลดไฟล์ latest.tar.gz มาโดยใช้คำสั่ง
```console
sudo tar -xzvf latest.tar.gz
```
***-xzvf*** เป็นคำสั่งคลายไฟล์ที่มีสกุลไฟล์ .gz
จากนั้นใช้คำสั่ง ลบไฟล์ ที่มีนามสกุล **.html , .gz , .php** โดยใช้
```console
sudo rm *.html *.gz *.php    //rm คือการ Remove 
```
เมื่อเสร็จแล้วเราจะทำการ rsync หรือการคัดลอกไฟล์ผ่านแฟ้ม wordpress เข้าไปวางใน directory ที่ต้องการ
```console
sudo rsync -av wordpress/* /var/www/html/
```
***-av*** <br>
-a คือ archive ทำให้ ไฟล์ตันฉบับกับไฟล์ปลายทางเหมือนกันทุกอย่าง<br>
-v คือ verbose ให้แสดงรายละเอียดระหว่างทำงานขึ้นมาบนหน้าจอ

ต่อมาจะเป็นการให้สิทธิ์กับผู้แก้ไขไฟล์โดย
```console
sudo chown -R www-data:www-data /var/www/html/
sudo chmod -R 755 /var/www/html
```
***chown*** คือคำสั่งที่ใช้ในเปลี่ยนเจ้าของ file หรือ directory รวมทั้ง file owner และ group owner อีกคำสั่งจะเป็นการแก้ permission โดย<br> 
**755** คือ **เจ้าของอ่าน เขียน และประมวลผลได้** แต่คนอื่นสามารถ**อ่าน**และ**ทำการประมวลผลได้เท่านั้น** 
```code
Execute : 4
Write : 2
Read : 1
```
เลขในหลักแรก เจ้าของ Files / Directory (Owner)<br>
เลขในหลักที่สอง ผู้ใช้กลุ่มเดียวกับเจ้าของ Files / Directory (Group)<br>
เลขในหลักที่สาม ผู้ใช้อื่น ๆ (Others / Puplic)<br>

ต่อมาจะเป็นการสร้าง Database ให้กับ WORDPRESS
```console
sudo mysql -u root –p แล้วกด Enter ไปหนึ่งครั้ง MariaDB จะถามหา password ที่เราได้ทำการใส่ไว้ในการติดตั้งก่อนหน้า
mysql> CREATE DATABASE <chosen_wp_database>; แล้วกด Enter
mysql> GRANT ALL PRIVILEGES ON <chosen_wp_database>.* TO '<chosen_username>'@'localhost' IDENTIFIED BY '<chosen_password>'; กด Enter
mysql> FLUSH PRIVILEGES; กด Enter
mysql> EXIT; กด Enter เพื่อออกจาก MariaDB
```
ตัวอย่างที่สร้าง
```console
sudo mysql -u root –p แล้วกด Enter ไปหนึ่งครั้ง MariaDB จะถามหา password ที่เราได้ทำการใส่ไว้ในการติดตั้งก่อนหน้า
mysql> CREATE DATABASE ABC;  
mysql> GRANT ALL PRIVILEGES ON ABC.* TO 'Sunat'@'localhost' IDENTIFIED BY 'SunatP1234';
mysql> FLUSH PRIVILEGES;
mysql> EXIT;
```
ทำการย้ายไฟล์ที่ทำให้ wordpress สามารถแก้ไขค่าได้โดย
```console
sudo mv wp-config-sample.php wp-config.php //mv คือ move
```

ทำการเรียก nano มาแก้ไขไฟล์ wp-config.php
```console
sudo nano wp-config.php
```



แก้ค่าตรงนี้ให้เป็นค่าที่เราสร้างไว้ใน MariaDB
```php
define( 'DB_NAME', 'ABC' );

/** MySQL database username */
define( 'DB_USER', 'Sunat' );

/** MySQL database password */
define( 'DB_PASSWORD', 'SunatP1234' );

/** MySQL hostname */
define( 'DB_HOST', 'localhost' );

/** Database Charset to use in creating database tables. */
define( 'DB_CHARSET', 'utf8' );

/** The Database Collate type. Don't change this if in doubt. */
define( 'DB_COLLATE', '' );
```

จากนั้นเพิ่มโค้ดตรงนี้ไป
```php
define('WP_HOME', '/');
define('WP_SITEURL', '/');
define('WP_MEMORY_LIMIT', '512M');
```
เพื่อให้สามารถเรียกหน้า WORDPRESS ได้ในกรณีที่เปลี่ยน IP Address จากนั้น Ctrl + X เพื่อทำการเซฟ แล้ว กด Y ตามด้วย Enter เพื่อบันทึกไฟล์ <br>
จากนั้นให้ใช้ Browser บน windows เพื่อทำการติดตั้ง Wordpress 
โดยพิมพ์เลข IP Address ตามด้วย /wp-admin/install.php

ตัวอย่าง
```console
http://192.168.XXX.XXX/wp-admin/install.php
```
จะพบกับหน้า install จากนั้นก็ติดตั้งไปเรื่อยๆกรอกข้อมูลให้ครบแล้วก็เป็นอันเสร็จ