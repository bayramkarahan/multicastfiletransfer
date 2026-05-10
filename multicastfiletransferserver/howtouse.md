/*
===========================================================
 Multicast Sender - Kullanım Rehberi
===========================================================

Bu uygulama 3 farklı modda çalışır:

-----------------------------------------------------------
1) PROJECT MODE (varsayılan)
-----------------------------------------------------------
Parametre verilmezse çalışır.

Kullanım:
    multicast_sender

Davranış:
    - Kaynak: ./aaa
    - Hedef: /tmp
    - Client listesi sabit (manuel IP listesi)
    - Dialog açılmaz

-----------------------------------------------------------
2) RIGHT CLICK MODE (Nemo / Dosya yöneticisi)
-----------------------------------------------------------

Nemo action örneği:
    Exec=/usr/bin/multicast_sender --mode rightclick --target ask %F

Alternatif targetlar:
    --target home
    --target desktop
    --target tmp
    --target ask        (kullanıcıdan klasör seçtirir)
    --target /custom/path

Örnekler:
    --mode rightclick --target home %F
    --mode rightclick --target ask %F

Davranış:
    - Seçilen dosya gönderilir (%F)
    - allowedClients = 0.0.0.0 (herkes kabul eder)
    - ask kullanılırsa dialog açılır

-----------------------------------------------------------
3) CONSOLE MODE (terminal)
-----------------------------------------------------------

Kullanım:
    multicast_sender --mode console --target home /path/file
    multicast_sender --mode console --target ask /path/file

Örnek:
    multicast_sender --mode console --target tmp /home/user/test.deb

Davranış:
    - Dosya parametreden alınır
    - allowedClients = 0.0.0.0
    - target=ask ise dialog açılır

-----------------------------------------------------------
TARGET PARAMETRELERİ
-----------------------------------------------------------

home        → kullanıcının home dizini
desktop     → Masaüstü
tmp         → /tmp
ask         → dialog açılır
/custom     → verilen path kullanılır

-----------------------------------------------------------
MULTICAST CLIENT KURALI
-----------------------------------------------------------

Server:
    allowedClients << "0.0.0.0";

Client:
    Eğer listede "0.0.0.0" varsa HERKES kabul edilir.

Örnek kontrol:
    allowed = allowedClients.isEmpty()
           || allowedClients.contains(myIp)
           || allowedClients.contains("0.0.0.0");

-----------------------------------------------------------
NOTLAR
-----------------------------------------------------------

- QFileDialog sadece rightclick ve console modunda açılır
- project modunda GUI müdahalesi yoktur
- %F birden fazla dosya gönderebilir (şu an tek dosya alınıyor)

===========================================================
*/
