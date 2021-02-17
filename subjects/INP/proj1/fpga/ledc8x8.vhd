library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu. 
       --MV: Dobře.
    SMCLK   : in std_logic;
    RESET   : in std_logic;
    ROW     : out std_logic_vector(0 to 7);
    --COL     : out std_logic_vector(0 to 7)
    LED     : out std_logic_vector(0 to 7)
);
end ledc8x8;

architecture main of ledc8x8 is

    -- Sem doplnte definice vnitrnich signalu. 
    -- MV: OK, doplním.
    
    signal CE_CNT: std_logic_vector(0 to 7);
    signal CE: std_logic; -- Povoleni pro posun řádku
    signal ROW_TMP: std_logic_vector(0 to 7); -- Pomocný vektor pro posouvání konkrétního řádku
begin

    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.

    -- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
    -- v souboru ledc8x8.ucf.
    -- MV: Provedeno. Snad. Možná. ... No jo jdu to tam doplnit.

    -- GENEROVANI FREKVENCE
    ce_gen: process(SMCLK, RESET)
    begin
        if RESET = '1' then
            CE_CNT <= "00000000";
        elsif SMCLK'event and SMCLK = '1' then
            CE_CNT <= CE_CNT + 1;
        end if;
    end process ce_gen;
    CE <= '1' when CE_CNT = X"FF" else '0';

    -- Převod vzbraného řádku na sloupce
    row_to_col: process(ROW_TMP)
    begin
        case ROW_TMP is
            when "10000000" => LED <= "11101101";
            when "01000000" => LED <= "11001001";
            when "00100000" => LED <= "10100101";
            when "00010000" => LED <= "01101101";
            when "00001000" => LED <= "10110111";
            when "00000100" => LED <= "10101111";
            when "00000010" => LED <= "10011111";
            when "00000001" => LED <= "10111111";
            when others => LED <= "11111111";
        end case;
    end process row_to_col;

    -- Posouvání řádku
    next_row: process(SMCLK, RESET)
    begin
        if RESET = '1' then
            ROW_TMP <= "10000000";
        elsif SMCLK'event and SMCLK = '1' then
            if CE = '1' then
                ROW_TMP <= ROW_TMP(7) & ROW_TMP(0 to 6);
            end if;
        end if;
    end process next_row;
    ROW <= ROW_TMP;

end main;
