-- -/\/\-\/\/- --

-- ---------------------------------------------- --
-- READ ME                                        --
-- ---------------------------------------------- --
-- Dynamicke SQL:                                 --
--   Pouzito poze pri vytvareni databaze,         --
--   kdy nevadi casove narocnejsi zpracovani SQL. --
-- ---------------------------------------------- --
-- Udelovani prav k objektum:                     --
--   Jelikoz pracuji sam jsou zde uvedeny jen     --
--   priklady jak na to. Jsou zakomentovany a je  --
--   zde pouzit imaginarni login xlogin00.        --
-- ---------------------------------------------- --

-- --------------------------------- --
-- Obsah:                            --
-- ================================= --
-- READ ME                           --
-- Smazani tabulek a seqenci         --
-- Vytvoreni tabulek                 --
-- Nastaveni cizich klicu            --
-- Triggery                          --
-- Vkladani zaznamu                  --
-- Select dotazy                     --
-- Ukazka triggeru, funkci, procedur --
-- Materializovany pohled            --
-- Udeleni prav k objektum           --
-- Explain plan, Index               --
-- --------------------------------- --


SET serveroutput ON;

-- ------------------------- --
-- Smazani tabulek a seqenci --
-- ------------------------- --

-- Procedura pro bezpecne smazani sekvenci.
-- Smaze sekvenci pokud existuje. 
CREATE OR REPLACE PROCEDURE DropSequenceIfExist ( name in varchar2 ) 
IS
BEGIN
	EXECUTE IMMEDIATE 'DROP SEQUENCE ' || name;
	DBMS_OUTPUT.put_line('Byla smazana tabulka: ' || name );
	EXCEPTION WHEN OTHERS THEN NULL;
	DBMS_OUTPUT.put_line('Tabulka jiz neexistuje: ' || name );
END;
/

-- Procedura pro bezpecne smazani tabulek. 
-- Smaze tabulku pokud existuje. 
CREATE OR REPLACE PROCEDURE DropTableIfExist ( name in varchar2 ) 
IS
BEGIN
	EXECUTE IMMEDIATE 'DROP TABLE ' || name || ' CASCADE CONSTRAINTS';
	DBMS_OUTPUT.put_line('Byla smazana sekvence: ' || name );
	EXCEPTION WHEN OTHERS THEN NULL;
	DBMS_OUTPUT.put_line('Seqence jiz neexistuje: ' || name );
END;
/

-- Smazani tabulek, ktere pouzivam. 
execute DropTableIfExist('Pozice');
execute DropTableIfExist('Charakteristika');
execute DropTableIfExist('TypLode');
execute DropTableIfExist('Aliance');
execute DropTableIfExist('Pirat');
execute DropTableIfExist('SeznamCharakteristik');
execute DropTableIfExist('Posadka');
execute DropTableIfExist('ClenemPosadky');
execute DropTableIfExist('TvoriAlianci');
execute DropTableIfExist('Pristav');
execute DropTableIfExist('AliancniSchuzka');
execute DropTableIfExist('Flotila');
execute DropTableIfExist('Lod');
execute DropTableIfExist('BitvaPosadka');
execute DropTableIfExist('BitvaAliance');
execute DropTableIfExist('LodVBitvaPosadka');
execute DropTableIfExist('LodVBitvaAliance');

-- Smazani sekvenci, ktere pouzivam. 
execute DropSequenceIfExist('Seq_Pozice');
execute DropSequenceIfExist('Seq_Charakteristika');
execute DropSequenceIfExist('Seq_TypLode');
execute DropSequenceIfExist('Seq_Aliance');
execute DropSequenceIfExist('Seq_Pirat');
execute DropSequenceIfExist('Seq_SeznamCharakteristik');
execute DropSequenceIfExist('Seq_Posadka');
execute DropSequenceIfExist('Seq_ClenemPosadky');
execute DropSequenceIfExist('Seq_TvoriAlianci');
execute DropSequenceIfExist('Seq_Pristav');
execute DropSequenceIfExist('Seq_AliancniSchuzka');
execute DropSequenceIfExist('Seq_Flotila');
execute DropSequenceIfExist('Seq_Lod');
execute DropSequenceIfExist('Seq_BitvaPosadka');
execute DropSequenceIfExist('Seq_BitvaAliance');
execute DropSequenceIfExist('Seq_LodVBitvaPosadka');
execute DropSequenceIfExist('Seq_LodVBitvaAliance');


-- Vyprazdneni kose. 
PURGE RECYCLEBIN;


-- ----------------- --
-- Vytvoreni tabulek --
-- ----------------- --

-- Mozne pozice v posadce, ktere muze pirat vykonavat.
CREATE SEQUENCE Seq_Pozice START WITH 1 INCREMENT BY 1;
CREATE TABLE Pozice (
	ID int DEFAULT Seq_Pozice.NEXTVAL NOT NULL,
	-- text jako PK? Asi spis ne. 
	Nazev nvarchar2(30) NOT NULL UNIQUE, 
	Popis nvarchar2(255),
	-- Primary key
	CONSTRAINT PK_Pozice PRIMARY KEY (ID)
);

-- Charakteristicka vlastnost/vzhled. 
-- napr.: Chybejici oko, papousek na rameni, drevena noha, ... 
CREATE SEQUENCE Seq_Charakteristika START WITH 1 INCREMENT BY 1;
CREATE TABLE Charakteristika (
	ID int DEFAULT Seq_Charakteristika.NEXTVAL NOT NULL,
	Nazev nvarchar2(50) NOT NULL,
	Popis nvarchar2(255),
	-- Primary key
	CONSTRAINT PK_Charakteristika PRIMARY KEY (ID)
);

-- Typ lode aneb o jakou tridu lode se jedna. 
-- napr.: karavela, brigantina, ... 
CREATE SEQUENCE Seq_TypLode START WITH 1 INCREMENT BY 1;
CREATE TABLE TypLode (
	ID int DEFAULT Seq_TypLode.NEXTVAL NOT NULL,
	-- text jako PK? Asi spis ne. 
	Nazev nvarchar2(50) NOT NULL UNIQUE,
	MaxLidi int NOT NULL,
	PozicProDela int DEFAULT 0 NOT NULL,
	PocetSteznu int DEFAULT 0 NOT NULL,
	Sirka int NOT NULL,
	Delka int NOT NULL,
	MinPonor int NOT NULL,
	MaxPonor int NOT NULL,
	-- Primary key
	CONSTRAINT PK_TypLode PRIMARY KEY (ID)
);

-- Aliance nekolika piratskych posadek 
CREATE SEQUENCE Seq_Aliance START WITH 1 INCREMENT BY 1;
CREATE TABLE Aliance (
	ID int DEFAULT Seq_Aliance.NEXTVAL NOT NULL,
	Jmeno nvarchar2(50) NOT NULL,
	DatumZalozeni Date NOT NULL,
	DatumZruseni Date,
	-- Primary key
	CONSTRAINT PK_Aliance PRIMARY KEY (ID)
);

-- Pirat 
CREATE SEQUENCE Seq_Pirat START WITH 1 INCREMENT BY 1;
CREATE TABLE Pirat (
	ID int DEFAULT Seq_Pirat.NEXTVAL NOT NULL,
	-- Cele jmeno pirata, pokud nejake ma. 
	Jmeno nvarchar2(50),
	Prezdivka nvarchar2(30) NOT NULL,
	DatumNarozeni Date,
	BarvaVousu varchar(6) DEFAULT 'FFFFFF',
	Login nvarchar2(30) NOT NULL UNIQUE,
	-- Check
		-- BarvaVousu zadana v RGB: 
	CONSTRAINT CHK_BarvaVousu CHECK (REGEXP_LIKE(BarvaVousu, '[1-90A-Fa-f]{6}') ),
	-- Foreign keys
		-- Pirat se nachazi na lodi: 
	ID_Lod int,
	-- Primary key
	CONSTRAINT PK_Pirat PRIMARY KEY (ID)
);

-- Seznam charakteristik, ktere pirat ma.
CREATE SEQUENCE Seq_SeznamCharakteristik START WITH 1 INCREMENT BY 1;
CREATE TABLE SeznamCharakteristik (
	ID int DEFAULT Seq_SeznamCharakteristik.NEXTVAL NOT NULL,
	-- Foreign keys
	ID_Pirat int NOT NULL,
	ID_Charakteristika int NOT NULL, 
	-- Primary key
	CONSTRAINT PK_SeznamCharakteristik PRIMARY KEY (ID)
);

-- Piratska posadka 
CREATE SEQUENCE Seq_Posadka START WITH 1 INCREMENT BY 1;
CREATE TABLE Posadka (
	ID int DEFAULT Seq_Posadka.NEXTVAL NOT NULL,
	-- text jako PK? Asi spis ne. 
	Jmeno nvarchar2(50) NOT NULL UNIQUE,
	JollyRoger nvarchar2(255), -- ? PIC nebo url ?
	-- Foreign keys
	ID_Kapitan int NOT NULL,
	-- Primary key
	CONSTRAINT PK_Posadka PRIMARY KEY (ID)
);

-- Prideleni pirata k posadce na danou pozici v posadce. 
CREATE SEQUENCE Seq_ClenemPosadky START WITH 1 INCREMENT BY 1;
CREATE TABLE ClenemPosadky (
	ID int DEFAULT Seq_ClenemPosadky.NEXTVAL NOT NULL,
	DatumVstupu Date NOT NULL,
	DatumOpusteni Date,
	-- Foreign keys
	ID_Pirat int NOT NULL,
	ID_Posadka int NOT NULL,
	ID_Pozice int NOT NULL,
	-- Primary key
	CONSTRAINT PK_ClenemPosadky PRIMARY KEY (ID)
);

-- Ktere posadky tvori jaka aliance 
CREATE SEQUENCE Seq_TvoriAlianci START WITH 1 INCREMENT BY 1;
CREATE TABLE TvoriAlianci (
	ID int DEFAULT Seq_TvoriAlianci.NEXTVAL NOT NULL,
	DatumVstupu Date NOT NULL,
	DatumOpusteni Date,
	-- Foreign keys
	ID_Posadka int NOT NULL,
	ID_Aliance int NOT NULL,
	-- Primary key
	CONSTRAINT PK_TvoriAlianci PRIMARY KEY (ID)
);

-- Pristavy 
CREATE SEQUENCE Seq_Pristav START WITH 1 INCREMENT BY 1;
CREATE TABLE Pristav (
	ID int DEFAULT Seq_Pristav.NEXTVAL NOT NULL,
	Jmeno nvarchar2(50) NOT NULL,
	NazevLokality nvarchar2(50) DEFAULT n'Neznama',
	KapacitaLodi int NOT NULL,
	SouradniceX int,
	SouradniceY int,
	-- Foreign keys
		-- Pristav patri do teritoria pasadky: 
	ID_Posadka int, 
	-- Primary key
	CONSTRAINT PK_Pristav PRIMARY KEY (ID)
);

-- Aliance poradaji schuzky a vzdy se urci kdy a kde.
CREATE SEQUENCE Seq_AliancniSchuzka START WITH 1 INCREMENT BY 1;
CREATE TABLE AliancniSchuzka (
	ID int DEFAULT Seq_AliancniSchuzka.NEXTVAL NOT NULL,
	Datum Date NOT NULL,
	Duvod nvarchar2(255) NOT NULL,
	-- Foreign keys
	ID_Pristav int NOT NULL,
	ID_Aliance int NOT NULL,
	-- Primary key
	CONSTRAINT PK_AliancniSchuzka PRIMARY KEY (ID)
);

-- Flotila 
-- Vice lodi se seskupuje do flotil. 
-- Flotila ma sveho kapitana, ktery rozdava rozkazy kapitanum lodi. 
CREATE SEQUENCE Seq_Flotila START WITH 1 INCREMENT BY 1;
CREATE TABLE Flotila (
	ID int DEFAULT Seq_Flotila.NEXTVAL NOT NULL,
	Jmeno nvarchar2(50) NOT NULL,
	-- Foreign keys
	ID_KapitanFlotily int NOT NULL, 
	-- Primary key
	CONSTRAINT PK_Flotila PRIMARY KEY (ID)
);

-- Lod 
-- Lod muze patrit posadce, byt soucasti flotily a take muze kotvit v urcitem pristavu 
-- KodLode = prvni 4pismena z typu_lode; rok 1.vypluti; prvnich 4 az 8 pismen z nazvu lode (bez bilich znaku);
-- pr: (Karavela, 1987-12-30, Bleda smrt) Kara-1987-Bleda 
CREATE SEQUENCE Seq_Lod START WITH 1 INCREMENT BY 1;
CREATE TABLE Lod (
	ID int DEFAULT Seq_Lod.NEXTVAL NOT NULL,
	KodLode nvarchar2(19) NOT NULL UNIQUE,
	Jmeno nvarchar2(30) NOT NULL,
	DatumPrvnihoVypluti Date NOT NULL,
	Stav nvarchar2(255),
	-- Foreign keys
	ID_TypLode int NOT NULL, 
		-- Ktera posadka vlastni lod: 
	ID_Posadka int, 
		-- Kapitan velici lodi: 
	ID_KapitanLode int, 
	ID_Flotila int, 
		-- Pristav kde lod kotvi / NULL je na moři: 
	ID_Pristav int, 
	-- Primary key
	CONSTRAINT PK_Lod PRIMARY KEY (ID)
);

-- Bitva Posadka vs Posadka
CREATE SEQUENCE Seq_BitvaPosadka START WITH 1 INCREMENT BY 1;
CREATE TABLE BitvaPosadka (
	ID int DEFAULT Seq_BitvaPosadka.NEXTVAL NOT NULL,
	Datum Date NOT NULL,
	ZtratyLidi int DEFAULT 0,
	ZtratyLodi int DEFAULT 0,
	Duvod nvarchar2(255),
	-- Foreign keys
		-- Dve posadky, ktere proti sobe bojuji: 
	ID_Posadka1 int NOT NULL,
	ID_Posadka2 int NOT NULL,
		-- Pristav, kde se bojuje / NULL boj na mori: 
	ID_Pristav int,
	-- Primary key
	CONSTRAINT PK_BitvaPosadka PRIMARY KEY (ID)
);

-- Bitva Aliance vs Aliance
CREATE SEQUENCE Seq_BitvaAliance START WITH 1 INCREMENT BY 1;
CREATE TABLE BitvaAliance (
	ID int DEFAULT Seq_BitvaAliance.NEXTVAL NOT NULL,
	Datum Date NOT NULL,
	ZtratyLidi int DEFAULT 0,
	ZtratyLodi int DEFAULT 0,
	Duvod nvarchar2(255),
	-- Foreign keys
		-- Dve aliance, ktere proti sobe bojuji: 
	ID_Aliance1 int NOT NULL,
	ID_Aliance2 int NOT NULL,
		-- Pristav, kde se bojuje / NULL boj na mori: 
	ID_Pristav int,
	-- Primary key
	CONSTRAINT PK_BitvaAliance PRIMARY KEY (ID)
);

-- Lode v bitve posadek 
CREATE SEQUENCE Seq_LodVBitvaPosadka START WITH 1 INCREMENT BY 1;
CREATE TABLE LodVBitvaPosadka (
	ID int DEFAULT Seq_LodVBitvaPosadka.NEXTVAL NOT NULL,
	-- Foreign keys
	ID_Lod int NOT NULL,
	ID_BitvaPosadka int NOT NULL,
	-- Primary key
	CONSTRAINT PK_LodVBitvaPosadka PRIMARY KEY (ID)
);

-- Lode v bitve alianci 
CREATE SEQUENCE Seq_LodVBitvaAliance START WITH 1 INCREMENT BY 1;
CREATE TABLE LodVBitvaAliance (
	ID int DEFAULT Seq_LodVBitvaAliance.NEXTVAL NOT NULL,
	-- Foreign keys
	ID_Lod int NOT NULL,
	ID_BitvaAliance int NOT NULL,
	-- Primary key
	CONSTRAINT PK_LodVBitvaAliance PRIMARY KEY (ID)
);


-- ---------------------- --
-- Nastaveni cizich klicu --
-- ---------------------- --

-- Procedura pro vytvareni cizich klicu. 
CREATE OR REPLACE PROCEDURE NewForeignKey ( table_name in varchar2, col_name in varchar2, ref_table in varchar2 ) 
IS
BEGIN
	EXECUTE IMMEDIATE 'ALTER TABLE ' || table_name 
	|| ' ADD CONSTRAINT FK_' || col_name || '_' || table_name 
	|| ' FOREIGN KEY (' || col_name || ') REFERENCES ' || ref_table || ' ON DELETE CASCADE';
	DBMS_OUTPUT.put_line('Vytvoren cizi klic v tabulce: ' || table_name );
	-- EXCEPTION WHEN OTHERS THEN NULL;
END;
/


-- Pirat 
EXECUTE NewForeignKey('Pirat','ID_Lod','Lod(ID)');
-- SeznamCharakteristik 
EXECUTE NewForeignKey('SeznamCharakteristik','ID_Pirat','Pirat(ID)');
EXECUTE NewForeignKey('SeznamCharakteristik','ID_Charakteristika','Charakteristika(ID)');
-- Posadka 
EXECUTE NewForeignKey('Posadka','ID_Kapitan','Pirat(ID)');
-- ClenemPosadky 
EXECUTE NewForeignKey('ClenemPosadky','ID_Pirat','Pirat(ID)');
EXECUTE NewForeignKey('ClenemPosadky','ID_Posadka','Posadka(ID)');
EXECUTE NewForeignKey('ClenemPosadky','ID_Pozice','Pozice(ID)');
-- TvoriAlianci 
EXECUTE NewForeignKey('TvoriAlianci','ID_Posadka','Posadka(ID)');
EXECUTE NewForeignKey('TvoriAlianci','ID_Aliance','Aliance(ID)');
-- Pristav 
EXECUTE NewForeignKey('Pristav','ID_Posadka','Posadka(ID)');
-- AliancniSchuzka 
EXECUTE NewForeignKey('AliancniSchuzka','ID_Pristav','Pristav(ID)');
EXECUTE NewForeignKey('AliancniSchuzka','ID_Aliance','Aliance(ID)');
-- Flotila 
EXECUTE NewForeignKey('Flotila','ID_KapitanFlotily','Pirat(ID)');
-- Lod 
EXECUTE NewForeignKey('Lod','ID_TypLode','TypLode(ID)');
EXECUTE NewForeignKey('Lod','ID_Posadka','Posadka(ID)');
EXECUTE NewForeignKey('Lod','ID_KapitanLode','Pirat(ID)');
EXECUTE NewForeignKey('Lod','ID_Flotila','Flotila(ID)');
EXECUTE NewForeignKey('Lod','ID_Pristav','Pristav(ID)');
-- BitvaPosadka 
EXECUTE NewForeignKey('BitvaPosadka','ID_Posadka1','Posadka(ID)');
EXECUTE NewForeignKey('BitvaPosadka','ID_Posadka2','Posadka(ID)');
EXECUTE NewForeignKey('BitvaPosadka','ID_Pristav','Pristav(ID)');
-- BitvaAliance 
EXECUTE NewForeignKey('BitvaAliance','ID_Aliance1','Aliance(ID)');
EXECUTE NewForeignKey('BitvaAliance','ID_Aliance2','Aliance(ID)');
EXECUTE NewForeignKey('BitvaAliance','ID_Pristav','Pristav(ID)');
-- LodVBitvaPosadka 
EXECUTE NewForeignKey('LodVBitvaPosadka','ID_Lod','Lod(ID)');
EXECUTE NewForeignKey('LodVBitvaPosadka','ID_BitvaPosadka','BitvaPosadka(ID)');
-- LodVBitvaAliance 
EXECUTE NewForeignKey('LodVBitvaAliance','ID_Lod','Lod(ID)');
EXECUTE NewForeignKey('LodVBitvaAliance','ID_BitvaAliance','BitvaAliance(ID)');


-- -------- --
-- Triggery --
-- -------- --

-- Funkce pro ktera vygeneruje jedinecny kod lode pro danou lod. 
-- Pokud ale tato akce nebude mozna vrati chybu. 
CREATE OR REPLACE FUNCTION GeneratorKodLode ( new_lod Lod%ROWTYPE ) RETURN nvarchar2
IS
	new_KodLode nvarchar2(19);
	zk_typ_lode nvarchar2(4);
	zk_rok_vypluti nvarchar2(4);
	zk_jmeno nvarchar2(8);
	typ_lode TypLode.Nazev%TYPE; 
	counter int;
	zaznam_lod Lod%ROWTYPE;
	CURSOR cursor_Lod IS SELECT * FROM Lod;
	konec boolean;
BEGIN
	-- Zjistim typ lode. (Karavela, ...)
	BEGIN
		SELECT Nazev INTO typ_lode
		FROM TypLode 
		WHERE ID=new_lod.ID_TypLode;
	EXCEPTION WHEN NO_DATA_FOUND THEN
		raise_application_error(-20100, 'Nebyl nalezen dany typ lode.');
	END;
	zk_typ_lode := SUBSTR(typ_lode, 1, 4);

	-- Rok prvniho vypluti lode. 
	zk_rok_vypluti := TO_CHAR(new_lod.DatumPrvnihoVypluti, 'yyyy');

	-- Jmeno lode. 
	FOR counter in 4..9
	LOOP 
		IF counter = 9 THEN
			raise_application_error(-20101, 'Nepodarilo se vytvorit unikatni kod pro lod.');
		END IF;
		zk_jmeno := SUBSTR( regexp_replace(new_lod.Jmeno, '[[:space:]]*', ''), 1, counter);
		IF (LENGTH(zk_jmeno) < 4) THEN 
			raise_application_error(-20102, 'Jmeno lode je kratsi nez 4 znaky.');
		END IF;

		-- Testovani unikatnosti kodu. 
		new_KodLode := zk_typ_lode || n'-' || zk_rok_vypluti || n'-' || zk_jmeno;

		konec := true;
		OPEN cursor_Lod;
		LOOP
			FETCH cursor_Lod INTO zaznam_lod;
			EXIT WHEN cursor_Lod%NOTFOUND; 
			IF (zaznam_lod.KodLode=new_KodLode) THEN
				konec:=false;
			END IF;
		END LOOP;
		CLOSE cursor_Lod;

		EXIT WHEN konec;

	END LOOP;

	DBMS_OUTPUT.put_line('KodLode: ' || new_KodLode );

	RETURN new_KodLode;
	-- EXCEPTION WHEN OTHERS THEN NULL;
END;
/

-- Funkce pro zjisteni zda je na lodi volne misto. 
create or replace PROCEDURE JeNaLodiMisto ( pID_Lod Lod.ID%TYPE )
IS
	kapacita_lode int;
	aktualne_na_lodi int;
BEGIN
	BEGIN
		SELECT TL.MaxLidi INTO kapacita_lode 
			FROM Lod L 
				JOIN TypLode TL ON L.ID_TypLode = TL.ID 
			WHERE L.ID = pID_Lod ;
	EXCEPTION WHEN NO_DATA_FOUND THEN
		raise_application_error(-20100, 'Nebyl nalezen hledany udaj.');
	END;
	BEGIN
        SELECT *  INTO aktualne_na_lodi FROM (
            SELECT COALESCE(COUNT(*),0)
			FROM Pirat Pir
			WHERE Pir.ID_Lod=pID_Lod
			GROUP BY Pir.ID_Lod
        ) WHERE rownum = 1;
	EXCEPTION WHEN OTHERS THEN 
		aktualne_na_lodi := 0;
	END;
	IF (kapacita_lode > aktualne_na_lodi) THEN
		DBMS_OUTPUT.put_line('Na lodi je misto. (' || aktualne_na_lodi || ' z ' || kapacita_lode || ')' );
	ELSE
		DBMS_OUTPUT.put_line('Na lodi neni misto. (' || aktualne_na_lodi || ' z ' || kapacita_lode || ')'  );
	END IF;
END;
/

-- Auto inkrementace pro tabulku Pozice 
CREATE OR REPLACE TRIGGER AutoIncrementPozice
BEFORE INSERT ON Pozice FOR EACH ROW
BEGIN
	IF (:NEW.ID IS NULL) THEN
		:NEW.ID := Seq_Pozice.NEXTVAL;
	END IF;
END;
/

-- Automaticke doplneni kodu lodi pokud nebyl zadan. 
CREATE OR REPLACE TRIGGER AutoChangeKodLode
BEFORE INSERT OR UPDATE ON Lod FOR EACH ROW
DECLARE
	new_lod Lod%ROWTYPE;
BEGIN
	IF (:NEW.KodLode IS NULL) THEN
		new_lod.Jmeno := :NEW.Jmeno;
		new_lod.DatumPrvnihoVypluti := :NEW.DatumPrvnihoVypluti;
		new_lod.ID_TypLode := :NEW.ID_TypLode;
		:NEW.KodLode := GeneratorKodLode(new_Lod);
	END IF;
END;
/


-- KodLode = prvni 4pismena z typu_lode; rok 1.vypluti; prvnich 4 az 8 pismen z nazvu lode ( bez bilich znaku );
-- pr: (Karavela, 1987-12-30, Bleda smrt) Kara-1987-Bleda 
-- KodLode nvarchar2(19) NOT NULL UNIQUE,
-- [4]-[4]-[4-8] 

-- Kontrola spravnosti kodu lode. 
CREATE OR REPLACE TRIGGER CheckKodLode
BEFORE INSERT OR UPDATE OF KodLode ON Lod FOR EACH ROW 
DECLARE
	kod_lode Lod.KodLode%TYPE;
	typ_lode TypLode.Nazev%TYPE; 
	datum_prvniho_vypluti Lod.DatumPrvnihoVypluti%TYPE;

	zk_typ_lode nvarchar2(4);
	zk_rok_vypluti nvarchar2(4);
	zk_jmeno nvarchar2(8);

BEGIN
	kod_lode := :NEW.kodLode;
	-- Kontrola odpovidajici velikosti pro kod lode. 
	IF (LENGTH(kod_lode) < 14 OR LENGTH(kod_lode) > 19 ) THEN
		raise_application_error(-20100, 'Delka kodu lode.');
	END IF;
	-- Kontrola oddelovacu v kodu lode. 
	IF (SUBSTR(kod_lode,5,1) != u'-' OR SUBSTR(kod_lode,10,1) != u'-') THEN 
		raise_application_error(-20101, 'Pomlcky v kodu lode.');
	END IF;
	-- Kontrola 1. casti - typ lode. 
	zk_typ_lode := SUBSTR(kod_lode,1,4);
	BEGIN
		SELECT Nazev INTO typ_lode
		FROM TypLode 
		WHERE ID=:NEW.ID_TypLode;
	EXCEPTION WHEN NO_DATA_FOUND THEN
		raise_application_error(-20103, 'Nebyl nalezen dany typ lod.e');
	END;
	IF ( zk_typ_lode != SUBSTR(typ_lode,1,4) ) THEN
		raise_application_error(-20104, 'Neschoduje se typ lode.');
	END IF;

	-- Kontrola 2. casti - rok vypluti. 
	zk_rok_vypluti := SUBSTR(kod_lode,6,4);
	IF ( zk_rok_vypluti != TO_CHAR(:NEW.DatumPrvnihoVypluti, 'yyyy') ) THEN
		raise_application_error(-20102, 'Neschoduje se rok vypluti.');
	END IF;

	-- Kontrola 3. casti - jmeno lode. 
	zk_jmeno := SUBSTR(kod_lode,11,8);
	IF ( zk_jmeno != SUBSTR( regexp_replace(:NEW.Jmeno, '[[:space:]]*', '') ,1,LENGTH(zk_jmeno)) ) THEN
		raise_application_error(-20104, 'Neschoduje se jmeno lode');
	END IF;
END;
/



-- ---------------- --
-- Vkladani zaznamu --
-- ---------------- --

-- Pozice 
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Kormidelník', n'' );
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Navigátor', n'' );
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Kuchař', n'' );
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Doktor', n'' );
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Loďař', n'' );
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Muzikant', n'' );
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Důstojník', n'' );
INSERT INTO Pozice (Nazev, Popis) 
	VALUES (n'Uklízeč', n'' );

-- Charakteristika 
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Slameny klobouk', n'' );
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Drevena prava noha', n'' );
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Drevena leva noha', n'' );
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Papousek na rameni', n'' );
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Hak misto prave ruky', n'' );
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Hak misto leve ruky', n'' );
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Paska pres prave oko', n'' );
INSERT INTO Charakteristika (Nazev, Popis) 
	VALUES (n'Paska pres leve oko', n'' );

-- TypLode 
INSERT INTO TypLode (Nazev, MaxLidi, PozicProDela, PocetSteznu, Sirka, Delka, MinPonor, MaxPonor) 
	VALUES (n'Karavela', 20, 8, 2, 15, 30, 1, 3 );
INSERT INTO TypLode (Nazev, MaxLidi, PozicProDela, PocetSteznu, Sirka, Delka, MinPonor, MaxPonor) 
	VALUES (n'Karaka', 30, 24, 3, 20, 40, 3, 5 );
INSERT INTO TypLode (Nazev, MaxLidi, PozicProDela, PocetSteznu, Sirka, Delka, MinPonor, MaxPonor) 
	VALUES (n'Galeona', 100, 100, 4, 40, 70, 4, 8 );
INSERT INTO TypLode (Nazev, MaxLidi, PozicProDela, PocetSteznu, Sirka, Delka, MinPonor, MaxPonor) 
	VALUES (n'Bark', 60, 60, 4, 30, 60, 4, 6 );
INSERT INTO TypLode (Nazev, MaxLidi, PozicProDela, PocetSteznu, Sirka, Delka, MinPonor, MaxPonor) 
	VALUES (n'Koga', 10, 2, 1, 10, 30, 1, 2 );
INSERT INTO TypLode (Nazev, MaxLidi, PozicProDela, PocetSteznu, Sirka, Delka, MinPonor, MaxPonor) 
	VALUES (n'Hulk', 15, 4, 2, 10, 30, 1, 3 );
INSERT INTO TypLode (Nazev, MaxLidi, PozicProDela, PocetSteznu, Sirka, Delka, MinPonor, MaxPonor) 
	VALUES (n'Knarr', 10, 0, 1, 3, 9, 1, 1 );

-- Pirat 
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-1', TO_DATE('1950-01-01','yyyy-mm-dd'), 'aaaaaa', n'pirat-1', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-2', TO_DATE('1954-11-07','yyyy-mm-dd'), '000000', n'pirat-2', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-3', TO_DATE('1945-12-21','yyyy-mm-dd'), '000000', n'pirat-3', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-4', TO_DATE('1953-06-11','yyyy-mm-dd'), '222222', n'pirat-4', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-5', TO_DATE('1943-09-15','yyyy-mm-dd'), '999999', n'pirat-5', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-6', TO_DATE('1957-02-27','yyyy-mm-dd'), 'ffffff', n'pirat-6', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-7', TO_DATE('1959-01-09','yyyy-mm-dd'), '00ff00', n'pirat-7', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-8', TO_DATE('1951-03-14','yyyy-mm-dd'), 'ff00ff', n'pirat-8', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-9', TO_DATE('1950-05-07','yyyy-mm-dd'), 'aa11aa', n'pirat-9', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-10', TO_DATE('1953-02-21','yyyy-mm-dd'), 'ff1122', n'pirat-10', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-11', TO_DATE('1958-12-06','yyyy-mm-dd'), '001100', n'pirat-11', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-12', TO_DATE('1960-04-22','yyyy-mm-dd'), '000000', n'pirat-12', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-13', TO_DATE('1911-04-12','yyyy-mm-dd'), '000000', n'pirat-13', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (n'Pirát Pirátský', n'Pirat-14', TO_DATE('1933-10-20','yyyy-mm-dd'), 'ffffff', n'pirat-14', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-15', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-15', NULL );
-- v2 
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-16', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-16', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-17', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-17', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-18', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-18', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-19', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-19', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-20', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-20', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-21', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-21', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-22', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-22', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-23', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-23', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-24', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-24', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-25', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-25', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-26', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-26', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-27', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-27', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-28', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-28', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-29', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-29', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-30', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-30', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-31', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-31', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-32', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-32', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-33', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-33', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-34', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-34', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-35', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-35', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-36', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-36', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-37', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-37', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-38', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-38', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-39', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-39', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-40', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-40', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-41', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-41', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-42', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-42', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-43', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-43', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-44', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-44', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-45', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-45', NULL );
INSERT INTO Pirat (Jmeno, Prezdivka, DatumNarozeni, BarvaVousu, Login, ID_Lod) 
	VALUES (NULL, n'Pirat-46', TO_DATE('1944-09-05','yyyy-mm-dd'), 'ff0000', n'pirat-46', NULL );

-- SeznamCharakteristik 
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika) 
	VALUES (1,1);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (1,3);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (2,2);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (2,8);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (2,5);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (3,4);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (4,7);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (5,2);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (6,1);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (6,2);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (7,3);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (8,1);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (8,3);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (8,4);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (8,7);
INSERT INTO SeznamCharakteristik (ID_Pirat, ID_Charakteristika)
	VALUES (9,4);

-- Posadka 
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-1',n'',1);
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-2',n'',2);
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-3',n'',3);
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-4',n'',4);
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-5',n'',5);
-- v2 
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-6',n'',16);
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-7',n'',26);
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-8',n'',36);
INSERT INTO Posadka (Jmeno, JollyRoger, ID_Kapitan) 
	VALUES (n'Posadka-9',n'',46);


-- ClenemPosadky 
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1980-01-01','yyyy-mm-dd'), NULL, 1, 1, 1);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1980-01-01','yyyy-mm-dd'), TO_DATE('	1989-02-02','yyyy-mm-dd'), 2, 1, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1980-01-01','yyyy-mm-dd'), TO_DATE('	1989-02-03','yyyy-mm-dd'), 3, 1, 3);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1980-01-01','yyyy-mm-dd'), TO_DATE('	1989-02-03','yyyy-mm-dd'), 4, 1, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1980-01-01','yyyy-mm-dd'), TO_DATE('	1989-02-02','yyyy-mm-dd'), 5, 1, 5);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-03','yyyy-mm-dd'), NULL, 2, 2, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-04','yyyy-mm-dd'), NULL, 3, 3, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-04','yyyy-mm-dd'), NULL, 4, 4, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-03','yyyy-mm-dd'), NULL, 5, 5, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-03','yyyy-mm-dd'), NULL, 6, 2, 1);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-04','yyyy-mm-dd'), NULL, 7, 3, 1);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-04','yyyy-mm-dd'), NULL, 8, 4, 1);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-02','yyyy-mm-dd'), NULL,  9, 1, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-03','yyyy-mm-dd'), NULL, 10, 2, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-03','yyyy-mm-dd'), NULL, 11, 3, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-03','yyyy-mm-dd'), NULL, 12, 4, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-02','yyyy-mm-dd'), NULL, 13, 1, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-05','yyyy-mm-dd'), NULL, 14, 2, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1989-02-05','yyyy-mm-dd'), NULL, 15, 3, 4);
-- v2 
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), NULL, 16, 6, 1);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 17, 6, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 18, 6, 3);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 19, 6, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 20, 6, 5);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 21, 6, 6);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 22, 6, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 23, 6, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 24, 6, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 25, 6, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), NULL, 26, 7, 1);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 27, 7, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 28, 7, 3);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 29, 7, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 30, 7, 5);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 31, 7, 6);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 32, 7, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 33, 7, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 34, 7, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 35, 7, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), NULL, 36, 8, 1);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 37, 8, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 38, 8, 3);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 39, 8, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 40, 8, 5);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 41, 8, 6);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 42, 8, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 43, 8, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 44, 8, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), TO_DATE('2000-01-01','yyyy-mm-dd'), 45, 8, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('1990-01-01','yyyy-mm-dd'), NULL, 46, 9, 1);


INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 17, 7, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 18, 7, 3);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 19, 7, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 20, 7, 5);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 21, 7, 6);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 22, 7, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 23, 7, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 24, 7, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 25, 7, 8);

INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 27, 6, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 28, 6, 3);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 29, 6, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 30, 8, 5);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 31, 8, 6);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 32, 8, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 33, 8, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 34, 8, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 35, 8, 8);

INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 37, 7, 2);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 38, 7, 3);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 39, 7, 4);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 40, 7, 5);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 41, 7, 6);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 42, 9, 7);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 43, 9, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 44, 9, 8);
INSERT INTO ClenemPosadky (DatumVstupu, DatumOpusteni, ID_Pirat, ID_Posadka, ID_Pozice) 
	VALUES (TO_DATE('2000-01-01','yyyy-mm-dd'), NULL, 45, 9, 8);



-- Aliance 
INSERT INTO Aliance (Jmeno, DatumZalozeni, DatumZruseni) 
	VALUES (n'Aliance-1', TO_DATE('1989-10-01','yyyy-mm-dd'), TO_DATE('1990-01-01','yyyy-mm-dd') );
INSERT INTO Aliance (Jmeno, DatumZalozeni, DatumZruseni) 
	VALUES (n'Aliance-2', TO_DATE('1990-06-10','yyyy-mm-dd'), NULL );
INSERT INTO Aliance (Jmeno, DatumZalozeni, DatumZruseni) 
	VALUES (n'Aliance-3', TO_DATE('1990-08-08','yyyy-mm-dd'), NULL );

-- TvoriAlianci 
INSERT INTO TvoriAlianci (DatumVstupu, DatumOpusteni, ID_Posadka, ID_Aliance) 
	VALUES (TO_DATE('1989-10-01','yyyy-mm-dd'), TO_DATE('1990-01-01','yyyy-mm-dd'), 4, 1 );
INSERT INTO TvoriAlianci (DatumVstupu, DatumOpusteni, ID_Posadka, ID_Aliance) 
	VALUES (TO_DATE('1989-10-01','yyyy-mm-dd'), TO_DATE('1989-12-30','yyyy-mm-dd'), 5, 1 );
INSERT INTO TvoriAlianci (DatumVstupu, DatumOpusteni, ID_Posadka, ID_Aliance) 
	VALUES (TO_DATE('1990-06-10','yyyy-mm-dd'), NULL, 1, 2 );
INSERT INTO TvoriAlianci (DatumVstupu, DatumOpusteni, ID_Posadka, ID_Aliance) 
	VALUES (TO_DATE('1990-06-10','yyyy-mm-dd'), NULL, 2, 2 );
INSERT INTO TvoriAlianci (DatumVstupu, DatumOpusteni, ID_Posadka, ID_Aliance) 
	VALUES (TO_DATE('1990-08-08','yyyy-mm-dd'), NULL, 3, 3 );
INSERT INTO TvoriAlianci (DatumVstupu, DatumOpusteni, ID_Posadka, ID_Aliance) 
	VALUES (TO_DATE('1990-08-08','yyyy-mm-dd'), NULL, 4, 3 );
INSERT INTO TvoriAlianci (DatumVstupu, DatumOpusteni, ID_Posadka, ID_Aliance) 
	VALUES (TO_DATE('1990-08-10','yyyy-mm-dd'), NULL, 5, 3 );

-- Pristav 
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Parapa', n'Ostrov Parapa',3,100,100, 1);
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Kurama', n'Kuramsky poloostrov',3,700,700, 1);
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Modre pristaviste', n'Proklata zatoka', 10, 650, 200, NULL);
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Cerna kotva', n'Pisecny ostruvek', 1, 300, 1000, NULL);
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Bude lip', n'Ztraceny ostrov', 5, NULL, NULL, 2);
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Včera bylo pozdě', n'Poloostrov Kamara',5,600,100, 3);
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Vidlákov', n'Poloostrov Kamara',4,600,111, 4);
INSERT INTO Pristav (Jmeno, NazevLokality, KapacitaLodi, SouradniceX, SouradniceY, ID_Posadka) 
	VALUES (n'Přístav ztraceného poutníka', n'Cestujici ostrov',10,NULL,NULL, NULL);

-- AliancniSchuzka 
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1989-10-10','yyyy-mm-dd'), n'Neni zadny duvod.', 1, 1 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1989-11-20','yyyy-mm-dd'), n'Neni zadny duvod.', 2, 1 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1989-12-30','yyyy-mm-dd'), n'Neni zadny duvod.', 2, 1 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1990-06-20','yyyy-mm-dd'), n'Pakt o neutoceni.', 1, 2 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1990-08-05','yyyy-mm-dd'), n'Spoluprace', 5, 2 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1990-08-10','yyyy-mm-dd'), n'Spoluprace', 3, 3 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1990-09-01','yyyy-mm-dd'), n'Sdileni inforamci.', 3, 3 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1990-10-01','yyyy-mm-dd'), n'Sdileni inforamci.', 3, 3 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1990-11-01','yyyy-mm-dd'), n'Sdileni inforamci.', 3, 3 );
INSERT INTO AliancniSchuzka (Datum, Duvod, ID_Pristav, ID_Aliance) 
	VALUES (TO_DATE('1990-12-01','yyyy-mm-dd'), n'Sdileni inforamci.', 3, 3 );

-- Flotila 
INSERT INTO Flotila (Jmeno, ID_KapitanFlotily) 
	VALUES (n'Spojenectvo Aliance-3', 3);

-- Lod 
-- KodLode = prvni 4pismena z typu_lode; rok 1.vypluti; prvnich 4 az 8 pismen z nazvu lode (bez bilich znaku);
-- pr: (Karavela, 1987-12-30, Bleda smrt) Kara-1987-Bleda 
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara-1950-Koca', n'Kocapka', TO_DATE('1950-01-01','yyyy-mm-dd'), n'Plavby schopna ... snad.', 1, 1, 1, NULL, NULL );
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara-1952-Piko', n'Pikolino', TO_DATE('1952-01-01','yyyy-mm-dd'), n'Plavby schopna.', 1, 2, 2, NULL, 3 );
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara-1940-Černá', n'Černá želva', TO_DATE('1940-01-01','yyyy-mm-dd'), n'Potřebovala by pár záplat.', 2, 3, 3, NULL, NULL );
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara-1954-Bambi', n'Bambina', TO_DATE('1954-01-01','yyyy-mm-dd'), n'Plavby schopna.', 2, 4, 4, NULL, NULL );
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Knar-1949-Pochroum', n'Pochroumaná Šipka', TO_DATE('1949-01-01','yyyy-mm-dd'), n'Plavby schopna.', 7, 5, 5, NULL, 1 );
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Gale-1970-Gigant', n'Gigant', TO_DATE('1970-01-01','yyyy-mm-dd'), n'Probíhaji opravy a upravy.', 3, 1, 13, NULL, NULL );

-- BitvaPosadka 
INSERT INTO BitvaPosadka (Datum, ZtratyLidi, ZtratyLodi, Duvod, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-05-01','yyyy-mm-dd'), 0, 0, n'Hadka.', 1, 2, 6 );
INSERT INTO BitvaPosadka (Datum, ZtratyLidi, ZtratyLodi, Duvod, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-07-01','yyyy-mm-dd'), 0, 0, n'Boj o pristav.', 3, 4, 7 );
INSERT INTO BitvaPosadka (Datum, ZtratyLidi, ZtratyLodi, Duvod, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1989-02-01','yyyy-mm-dd'), 0, 0, n'Vzpoura.', 1, 1, NULL );
-- v2 
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-02 ','yyyy-mm-dd'), 6, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-03 ','yyyy-mm-dd'), 6, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-04 ','yyyy-mm-dd'), 6, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-05 ','yyyy-mm-dd'), 7, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-06 ','yyyy-mm-dd'), 7, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-07 ','yyyy-mm-dd'), 7, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-08 ','yyyy-mm-dd'), 8, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-09 ','yyyy-mm-dd'), 8, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-10 ','yyyy-mm-dd'), 8, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-11 ','yyyy-mm-dd'), 9, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-12 ','yyyy-mm-dd'), 9, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-02-13 ','yyyy-mm-dd'), 9, 8, 1 );

INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-02 ','yyyy-mm-dd'), 6, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-03 ','yyyy-mm-dd'), 6, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-04 ','yyyy-mm-dd'), 6, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-05 ','yyyy-mm-dd'), 7, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-06 ','yyyy-mm-dd'), 7, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-07 ','yyyy-mm-dd'), 7, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-08 ','yyyy-mm-dd'), 8, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-09 ','yyyy-mm-dd'), 8, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-10 ','yyyy-mm-dd'), 8, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-11 ','yyyy-mm-dd'), 9, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-12 ','yyyy-mm-dd'), 9, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('1990-03-13 ','yyyy-mm-dd'), 9, 8, 1 );

INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-02 ','yyyy-mm-dd'), 6, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-03 ','yyyy-mm-dd'), 6, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-04 ','yyyy-mm-dd'), 6, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-05 ','yyyy-mm-dd'), 7, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-06 ','yyyy-mm-dd'), 7, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-07 ','yyyy-mm-dd'), 7, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-08 ','yyyy-mm-dd'), 8, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-09 ','yyyy-mm-dd'), 8, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-10 ','yyyy-mm-dd'), 8, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-11 ','yyyy-mm-dd'), 9, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-12 ','yyyy-mm-dd'), 9, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-02-13 ','yyyy-mm-dd'), 9, 8, 1 );

INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-02 ','yyyy-mm-dd'), 6, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-03 ','yyyy-mm-dd'), 6, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-04 ','yyyy-mm-dd'), 6, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-05 ','yyyy-mm-dd'), 7, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-06 ','yyyy-mm-dd'), 7, 8, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-07 ','yyyy-mm-dd'), 7, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-08 ','yyyy-mm-dd'), 8, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-09 ','yyyy-mm-dd'), 8, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-10 ','yyyy-mm-dd'), 8, 9, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-11 ','yyyy-mm-dd'), 9, 6, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-12 ','yyyy-mm-dd'), 9, 7, 1 );
INSERT INTO BitvaPosadka (Datum, ID_Posadka1, ID_Posadka2, ID_Pristav) 
	VALUES (TO_DATE('2000-03-13 ','yyyy-mm-dd'), 9, 8, 1 );


-- BitvaAliance 
INSERT INTO BitvaAliance (Datum, ZtratyLidi, ZtratyLodi, Duvod, ID_Aliance1, ID_Aliance2, ID_Pristav) 
	VALUES (TO_DATE('1991-01-01','yyyy-mm-dd'), 0, 1, n'Boj o nadvladu.', 3, 2, NULL );

-- LodVBitvaPosadka 
INSERT INTO LodVBitvaPosadka (ID_Lod, ID_BitvaPosadka) 
	VALUES ( 1, 1 );
INSERT INTO LodVBitvaPosadka (ID_Lod, ID_BitvaPosadka) 
	VALUES ( 2, 1 );
INSERT INTO LodVBitvaPosadka (ID_Lod, ID_BitvaPosadka) 
	VALUES ( 4, 2 );
INSERT INTO LodVBitvaPosadka (ID_Lod, ID_BitvaPosadka) 
	VALUES ( 3, 2 );
INSERT INTO LodVBitvaPosadka (ID_Lod, ID_BitvaPosadka) 
	VALUES ( 1, 3 );

-- LodVBitvaAliance 
INSERT INTO LodVBitvaAliance (ID_Lod, ID_BitvaAliance) 
	VALUES ( 1, 1 );
INSERT INTO LodVBitvaAliance (ID_Lod, ID_BitvaAliance) 
	VALUES ( 2, 1 );
INSERT INTO LodVBitvaAliance (ID_Lod, ID_BitvaAliance) 
	VALUES ( 3, 1 );
INSERT INTO LodVBitvaAliance (ID_Lod, ID_BitvaAliance) 
	VALUES ( 4, 1 );
INSERT INTO LodVBitvaAliance (ID_Lod, ID_BitvaAliance) 
	VALUES ( 5, 1 );
INSERT INTO LodVBitvaAliance (ID_Lod, ID_BitvaAliance) 
	VALUES ( 6, 1 );

-- Umisteni piratu na lode. 
UPDATE Pirat SET ID_Lod = 1 WHERE ID IN (1, 9); 
UPDATE Pirat SET ID_Lod = 2 WHERE ID IN (2, 6, 10, 14); 
UPDATE Pirat SET ID_Lod = 3 WHERE ID IN (3, 7, 11, 15); 
UPDATE Pirat SET ID_Lod = 4 WHERE ID IN (4, 8, 12); 
UPDATE Pirat SET ID_Lod = 5 WHERE ID = 5; 
UPDATE Pirat SET ID_Lod = 6 WHERE ID = 13; 

-- ------------- --
-- Select dotazy --
-- ------------- --

--X 2 dotazy pro spojeni 2 tabulek 
--X 1 dotaz pro spojeni 3 tabulek 
--X 2 dotazy s klauzuli GROUP BY a agragacni funkci 
--X 1 dotaz s predikatem EXIST 
--X 1 dotaz s predikatem IN s vnorenym SELECT 
--X Komentare ke kazdemu dotazu - Jaka data hleda, jaka je jeho funkce
-- 


-- Spojeni 2 tabulek --
-- ================= --

-- Vypise seznam lodi (Kod lode, Jmeno lode) a prezdivka kapitana, ktery dane lodi veli. Serazeno dle kodu lode. 
SELECT L.KodLode, L.Jmeno AS Jmeno_Lode, P.Prezdivka AS Kapitan_Lode
	FROM Lod L 
		JOIN Pirat P 
			ON L.ID_KapitanLode = P.ID
	ORDER BY L.KodLode;


-- Vypise kompledni seznam Pristavu (Jmeno pristavu) a k nim inforamci o tom komu dany pristav patri. tj. do teritoria, ktere posadny pristav patri. (Jmeno posadky) Serazeno dle Jmena pristavu. 
SELECT Pri.Jmeno AS Jmeno_Pristavu, Pos.Jmeno AS Teritorium_Posadky
	FROM Pristav Pri
		LEFT JOIN Posadka Pos 
			ON Pri.ID_Posadka = Pos.ID
	ORDER BY Pri.Jmeno;


-- Vypise seznam charakteristik (Nazev charakteristiky), ktere ma pirat s id=8. Serazeno dle nazvu charakteristiky. 
SELECT C.Nazev
	FROM Charakteristika C 
		JOIN SeznamCharakteristik S 
			ON S.ID_Charakteristika = C.ID 
	WHERE 8 = S.ID_Pirat
	ORDER BY C.Nazev;


-- Spojeni 3 tabulek --
-- ================= --

-- Vypise ktere charakteristiky maji kteri pirati (Nazev charakteristiky, ID pirata a prezdivka pirata) a seznam se pote seradi dle nazvu charakteristiky a nasledne dle id pirta. 
SELECT C.Nazev AS Nazev_Charakteristiky, P.ID AS ID_Pirata, P.Prezdivka AS Prezdivka_Pirata
	FROM Pirat P 
		JOIN SeznamCharakteristik S 
			ON P.ID=S.ID_Pirat
		JOIN Charakteristika C 
			ON C.ID=S.ID_Charakteristika
	ORDER BY C.Nazev, P.ID;

-- Vypise seznam piratu, kteri jsou AKTUALNE cleny piratske 
-- posadky se jmenem 'Posadka-1' a kolik let jiz v posadce jsou. 
SELECT Pir.ID, Pir.Prezdivka, Pir.Jmeno, FLOOR((CURRENT_DATE - CP.DatumVstupu)/365) AS ROKU_V_POSADCE
	FROM Pirat Pir 
		JOIN ClenemPosadky CP 
			ON Pir.ID=CP.ID_Pirat
		JOIN Posadka Pos 
			ON Pos.ID=CP.ID_Posadka
	WHERE Pos.Jmeno = 'Posadka-1' 
		AND CP.DatumOpusteni IS NULL
	ORDER BY Pir.Prezdivka;

-- Vypise seznam piratu, kteri jsou BYLI cleny piratske 
-- posadky se jmenem 'Posadka-1' a kolik let byli v posadce jsou. 
SELECT Pir.ID, Pir.Prezdivka, Pir.Jmeno, FLOOR((CP.DatumOpusteni - CP.DatumVstupu)/365) AS ROKU_V_POSADCE
	FROM Pirat Pir 
		JOIN ClenemPosadky CP 
			ON Pir.ID=CP.ID_Pirat
		JOIN Posadka Pos 
			ON Pos.ID=CP.ID_Posadka
	WHERE Pos.Jmeno = 'Posadka-1' 
		AND CP.DatumOpusteni IS NOT NULL
	ORDER BY Pir.Prezdivka;


-- GROUP BY + Agregacni funkce --
-- =========================== --

-- Vypise seznam bitev od nejnovejsi po nejstarsi. 
-- U bitev se vypisuje o jaky typ bitvy jde (Posadka vs Posadka, Aliance vs Aliance),
-- ID dane bitvy a pocet lodi, ktere se danych bitev zucastnily.
SELECT BP.Datum, 'Posadka vs Posadka' AS Typ_Bitvy, BP.ID AS ID_Bitvy, count(L.ID) AS Pocet_Lodi_v_Bitve
	FROM BitvaPosadka BP 
		LEFT JOIN LodVBitvaPosadka LBP 
			ON BP.ID=LBP.ID_BitvaPosadka
		LEFT JOIN Lod L 
			ON L.ID=LBP.ID_Lod
	GROUP BY BP.ID, BP.Datum
UNION
SELECT BA.Datum, 'Aliance vs Aliance' AS Typ_Bitvy, BA.ID AS ID_Bitvy, count(L.ID) AS Pocet_Lodi_v_Bitve
	FROM BitvaAliance BA
		LEFT JOIN LodVBitvaAliance LBA
			ON BA.ID=LBA.ID_BitvaAliance
		LEFT JOIN Lod L
			ON L.ID=LBA.ID_Lod
	GROUP BY BA.ID, BA.Datum
ORDER BY Datum Desc;


-- Vypise kolik max lidi se muze plavit na lodich pro jednotlive posadky 
-- a seradi seznam od nejvedsi kapaciti po nejmensi.
-- Pokud vice posadek ma stejne velkou kapacitu seradi je dle nazvu posadky. 
SELECT Pos.Jmeno,  COALESCE(SUM(T.MaxLidi),0) AS Max_Pozic_Pro_Lidi
	FROM Posadka Pos 
		LEFT JOIN Lod L 
			ON Pos.ID=L.ID_Posadka
		LEFT JOIN TypLode T 
			ON L.ID_TypLode=T.ID
	GROUP BY Pos.Jmeno
	ORDER BY Max_Pozic_Pro_Lidi Desc, Pos.Jmeno;



-- EXIST a vnoreny SELECT --
-- ====================== --

-- Vypise pristavy ve kterych se nekdy konala nejaka aliancni schuzka. 
SELECT P.ID, P.Jmeno
	FROM Pristav P
	WHERE EXISTS (
		SELECT *
			FROM AliancniSchuzka A
			WHERE P.ID = A.ID_Pristav
		);

-- IN a vnoreny SELECT --
-- =================== --

-- Vypise seznam lodi, ktere kotvi v pristavu, ktery nepatri do teritoria posadky doticne lode. 
-- Lode ktere nekotvi nikde (pluji po mori) nejsou vypsany.
SELECT *
	FROM Lod L 
	WHERE L.ID_Pristav NOT IN (
			SELECT P.ID
				FROM Pristav P
				WHERE L.ID_Posadka = P.ID_Posadka
		);


-- --------------------------------- --
-- Ukazka triggeru, funkci, procedur --
-- --------------------------------- --

-- TRIGGER CheckKodLode --
-- ==================== --

-- Vlozeni NEPLATNEHO kodu lode. Chybne oddelovace. 
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara+1000*LodD', n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- Vlozeni NEPLATNEHO kodu lode. Chybny rok. 
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara-3000-LodDu', n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- Vlozeni NEPLATNEHO kodu lode. Chybny typ lode. 
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Meka-1000-LodDuch', n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- Vlozeni NEPLATNEHO kodu lode. Chybne jmeno. 
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara-1000-Lodka', n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- Vlozeni PLATNEHO kodu lode.
INSERT INTO Lod (KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Kara-1000-LodD', n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );


-- TRIGGER AutoChangeKodLode            --
-- obsahujici FUNCTION GeneratorKodLode --
-- ==================================== --

SELECT L.KodLode AS Kod_Lode, TL.Nazev AS Nazev_Typu, L.DatumPrvnihoVypluti AS Datum_1_Vypluti, L.Jmeno AS Jmeno_Lode
FROM Lod L
	JOIN TypLode TL 
		ON TL.ID=L.ID_TypLode;

-- Vlozeni bez zadani kodu lode. Autovygenerovani. 
-- 2. OK 
INSERT INTO Lod ( Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- 3. OK 
INSERT INTO Lod ( Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- 4. OK 
INSERT INTO Lod ( Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- 5. OK 
INSERT INTO Lod ( Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );
-- 6. KO - Nepodari se vygenerovat kod. 
INSERT INTO Lod ( Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (n'Lod Duchu', TO_DATE('1000-01-01','yyyy-mm-dd'), n'Nezvestna.', 1, NULL, NULL, NULL, NULL );

SELECT L.KodLode AS Kod_Lode, TL.Nazev AS Nazev_Typu, L.DatumPrvnihoVypluti AS Datum_1_Vypluti, L.Jmeno AS Jmeno_Lode
FROM Lod L
	JOIN TypLode TL 
		ON TL.ID=L.ID_TypLode;


-- TRIGGER AutoIncrementPozice --
-- =========================== --
SELECT MAX(ID) AS Max_ID_Pozice FROM Pozice;
INSERT INTO Pozice (Nazev) VALUES (n'Nemehlo');
SELECT ID AS ID_Nove_Pozice FROM Pozice WHERE Nazev=n'Nemehlo';


-- TRIGGER JeNaLodiMisto --
-- ===================== --
INSERT INTO Lod (ID, KodLode, Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES (1000, n'Kara-2000-Test', n'TestX', TO_DATE('2000-01-01','yyyy-mm-dd'), n'', 1, NULL, NULL, NULL, NULL );

SET serveroutput ON;

execute JeNaLodiMisto( 1000 );

UPDATE Pirat SET ID_Lod = 1000 WHERE ID BETWEEN 16 AND 35; 

execute JeNaLodiMisto( 1000 );

-- PROCEDURE:           --
-- DropSequenceIfExist  --
-- DropTableIfExist     --
-- NewForeignKey        --
-- ==================== --
-- Tyto procedury je mozne vide pouzite v kodech vyse. 


-- ---------------------- --
-- Materializovany pohled --
-- ---------------------- --

-- Smazani materializovaneho pohledu. 
DROP MATERIALIZED VIEW LOG ON Lod;
DROP MATERIALIZED VIEW LOG ON Pirat;
DROP MATERIALIZED VIEW View_KapitanLode;

-- Vytvoreni materializovaneho pohledu. 
CREATE MATERIALIZED VIEW LOG ON Lod WITH PRIMARY KEY, ROWID(ID_KapitanLode);
CREATE MATERIALIZED VIEW LOG ON Pirat WITH PRIMARY KEY, ROWID(Prezdivka);

CREATE MATERIALIZED VIEW View_KapitanLode 
	CACHE					 -- Casem optimalizuje cteni pohledu. 
	BUILD IMMEDIATE			 -- Naplneni pohledu po vytvoreni. 
	REFRESH FAST ON COMMIT	 -- Obnoveni pohledu pri commitu. 
    ENABLE QUERY REWRITE 	 -- Bude pouzivan optimalizatorem. 
	-- Vybere Lode a vypise kolik lidi se na nich nachazi. 
	AS 
    SELECT Pir.ID, Pir.Prezdivka
        FROM Pirat Pir
        WHERE EXISTS (
            SELECT * FROM Lod L
            WHERE Pir.ID=L.ID_KapitanLode);
    

-- Vypisani pohledu. 
SELECT * from View_KapitanLode;
-- Pridani nove lode s novim kapitanem. 
INSERT INTO Lod ( Jmeno, DatumPrvnihoVypluti, Stav, ID_TypLode, ID_Posadka, ID_KapitanLode, ID_Flotila, ID_Pristav) 
	VALUES ( n'XXX YYY ZZZ', TO_DATE('1111-11-11','yyyy-mm-dd'), n'XYZ.', 3, NULL, 10, NULL, NULL );
-- Obnoveni pohledu. 
COMMIT;
-- Kontrola po pridani lode. 
SELECT * from View_KapitanLode;


-- ------------------------------------------ --
-- Udeleni prav k objektum pro napr. xlogin00 --
-- ------------------------------------------ --

-- Definovani prav pro uzivatele xlogin00 --
-- ====================================== --
-- -- Muze upravovat zaznami piratu. 
-- GRANT SELECT, UPDATE ON Pirat TO xlogin00; 
-- -- Muze libovolne manipulovat s charakteristikami 
-- GRANT ALL ON Charakteristika TO xlogin00; 
-- GRANT ALL ON SeznamCharakteristik TO xlogin00; 
-- GRANT ALL ON Lod TO xlogin00; 
-- -- Muze cist informace o posadkach a jejich clenech. 
-- GRANT SELECT ON Posadka TO xlogin00; 
-- GRANT SELECT ON ClenemPosadky TO xlogin00; 
-- -- Muze spoustet nasledujici procedury. 
-- GRANT EXECUTE ON JeNaLodiMisto TO xlogin00; 

-- -- Prideleni prav pro praci s Materializovanym pohledem. 
-- GRANT ALL ON View_KapitanLode TO xlogin00; 

-- Pouziti ze strany uzivatele xlogin00 --
-- ==================================== --
-- -- A) Pred vse davat login vlastnika. 
-- xvalka05.JeNaLodiMisto(...); 
-- SELECT * FROM xvalka05.Pirat; 
-- -- B) Vytvorit synonyma. 
-- CREATE SYNONYM JeNaLodiMisto FOR xvalka05.JeNaLodiMisto; 

-- Zruseni prav uzivateli xlogin00 --
-- =============================== --
-- REVOKE ALL ON JeNaLodiMisto FROM xlogin00 
-- REVOKE DELETE ON Charakteristika FROM xlogin00 
-- REVOKE DELETE ON SeznamCharakteristik FROM xlogin00 
-- REVOKE DELETE ON Lod FROM xlogin00 


-- ------------------- --
-- Explain plan, Index --
-- ------------------- --

-- Smazani indexu. 
DROP INDEX index_ClenemPosadky_ID_Posadka;
DROP INDEX index_ClenemPosadky_DatumVstupu;
DROP INDEX index_ClenemPosadky_ID_Pirat;
-- DROP INDEX index_TvoriAlianci_ID_Posadka;
-- DROP INDEX index_TvoriAlianci_ID_Aliance;

-- Pocet clenu posadky v dobe konani bitvy. 
EXPLAIN PLAN FOR
SELECT 'P' AS Typ_Bitvy, BP.ID AS ID_Bitvy, BP.Datum AS Datum_Bitvy, Pos.Jmeno AS Jmeno_Posadky, count(CP.ID_Pirat) as Piratu_V_Posadce
	FROM BitvaPosadka BP 
		JOIN Posadka Pos 
			ON Pos.ID=BP.ID_Posadka1 OR Pos.ID=BP.ID_Posadka2
		JOIN ClenemPosadky CP 
			ON Pos.ID=CP.ID_Posadka
	WHERE BP.Datum BETWEEN CP.DatumVstupu AND COALESCE(CP.DatumOpusteni,CURRENT_DATE)
	GROUP BY BP.ID, BP.Datum, Pos.Jmeno 
UNION 
SELECT 'A' AS Typ_Bitvy, BA.ID AS ID_Bitvy, BA.Datum AS Datum_Bitvy, Pos.Jmeno AS Jmeno_Posadky, count(CP.ID_Pirat) as Piratu_V_Posadce
	FROM BitvaAliance BA 
		JOIN Aliance Ali  
			ON Ali.ID=BA.ID_Aliance1 OR Ali.ID=BA.ID_Aliance2 
		JOIN TvoriAlianci TA 
			ON Ali.ID=TA.ID_Aliance 
		JOIN Posadka Pos 
			ON Pos.ID=TA.ID_Posadka
		JOIN ClenemPosadky CP 
			ON Pos.ID=CP.ID_Posadka
	WHERE BA.Datum BETWEEN CP.DatumVstupu AND COALESCE(CP.DatumOpusteni,CURRENT_DATE)
	GROUP BY BA.ID, BA.Datum, Pos.Jmeno 
ORDER BY Datum_Bitvy Desc;
SELECT * FROM TABLE(DBMS_XPLAN.display);

-- Vytvoreni Indexu. 
CREATE INDEX index_ClenemPosadky_ID_Posadka ON ClenemPosadky (ID_Posadka);
-- CREATE INDEX index_TvoriAlianci_ID_Posadka ON TvoriAlianci (ID_Posadka);
-- CREATE INDEX index_TvoriAlianci_ID_Aliance ON TvoriAlianci (ID_Aliance);


-- Pocet clenu posadky v dobe konani bitvy. 
EXPLAIN PLAN FOR
SELECT 'P' AS Typ_Bitvy, BP.ID AS ID_Bitvy, BP.Datum AS Datum_Bitvy, Pos.Jmeno AS Jmeno_Posadky, count(CP.ID_Pirat) as Piratu_V_Posadce
	FROM BitvaPosadka BP 
		JOIN Posadka Pos 
			ON Pos.ID=BP.ID_Posadka1 OR Pos.ID=BP.ID_Posadka2
		JOIN ClenemPosadky CP 
			ON Pos.ID=CP.ID_Posadka
	WHERE BP.Datum BETWEEN CP.DatumVstupu AND COALESCE(CP.DatumOpusteni,CURRENT_DATE)
	GROUP BY BP.ID, BP.Datum, Pos.Jmeno 
UNION 
SELECT 'A' AS Typ_Bitvy, BA.ID AS ID_Bitvy, BA.Datum AS Datum_Bitvy, Pos.Jmeno AS Jmeno_Posadky, count(CP.ID_Pirat) as Piratu_V_Posadce
	FROM BitvaAliance BA 
		JOIN Aliance Ali  
			ON Ali.ID=BA.ID_Aliance1 OR Ali.ID=BA.ID_Aliance2 
		JOIN TvoriAlianci TA 
			ON Ali.ID=TA.ID_Aliance 
		JOIN Posadka Pos 
			ON Pos.ID=TA.ID_Posadka
		JOIN ClenemPosadky CP 
			ON Pos.ID=CP.ID_Posadka
	WHERE BA.Datum BETWEEN CP.DatumVstupu AND COALESCE(CP.DatumOpusteni,CURRENT_DATE)
	GROUP BY BA.ID, BA.Datum, Pos.Jmeno 
ORDER BY Datum_Bitvy Desc;
SELECT * FROM TABLE(DBMS_XPLAN.display);

--


-- Pocet clenu posadky v dobe konani bitvy. 
EXPLAIN PLAN FOR 
SELECT CP.DatumVstupu, COUNT(Pir.Prezdivka) as Pripojilo_se  
	FROM Pirat Pir 
		JOIN ClenemPosadky CP 
			ON Pir.ID = CP.ID_Pirat
	GROUP BY CP.DatumVstupu
ORDER BY CP.DatumVstupu Desc;
SELECT * FROM TABLE(DBMS_XPLAN.display);

-- Vytvoreni Indexu. 
CREATE INDEX index_ClenemPosadky_DatumVstupu ON ClenemPosadky (DatumVstupu);
CREATE INDEX index_ClenemPosadky_ID_Pirat ON ClenemPosadky (ID_Pirat);
-- Podle datumu se v dotazu seskupuje a nasledne řadí.


-- Pocet clenu posadky v dobe konani bitvy. 
EXPLAIN PLAN FOR 
SELECT CP.DatumVstupu, COUNT(Pir.Prezdivka) as Pripojilo_se  
	FROM Pirat Pir 
		JOIN ClenemPosadky CP 
			ON Pir.ID = CP.ID_Pirat
	GROUP BY CP.DatumVstupu
ORDER BY CP.DatumVstupu Desc;
SELECT * FROM TABLE(DBMS_XPLAN.display);
