-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

   -- -------------- --
   --  Pro Registry  --
   -- -------------- --

   -- PC
   signal registr_pc_inc: std_logic;
   signal registr_pc_dec: std_logic;
   signal registr_pc_codeAddr: std_logic_vector(11 downto 0); -- Pamet ROM bere 12bitovou adresu
   --signal registr_pc_codeAddr: std_logic_vector(0 to 11);

   -- CNT
   signal registr_cnt_inc: std_logic;
   signal registr_cnt_dec: std_logic;
   signal registr_cnt_value: std_logic_vector(11 downto 0); -- Pro cykly - muze mit libovolna velikost ja zvoliv 12 bitu

   -- PTR
   signal registr_ptr_inc: std_logic;
   signal registr_ptr_dec: std_logic;
   signal registr_ptr_dataAddr: std_logic_vector(9 downto 0); -- Pamet RAM bere 10bitovou adresu

   -- --------------------------- --
   --  Brainlove instrukcni sada  --
   -- --------------------------- --
   type instruction_set is (
      ptr_inc,     -- >
      ptr_dec,     -- <
      val_inc,     -- +
      val_dec,     -- -
      while_start, -- [
      while_end,   -- ]
      putchar,     -- .
      getchar,     -- ,
      break,       -- ~
      halt,        -- null
      nop          -- nedelej nic

   );
   signal instruction : instruction_set; -- Nactena instrukce


   -- -------------------------------------- --
   --  Multiplexor pro vyber ukladanych dat  --
   -- -------------------------------------- --
   signal sel_mx_wdata : std_logic_vector(1 downto 0); -- 2bity pro vyber vkladane hodnoty

   type fsm_state_type is (
      STATE_IDLE, STATE_FETCH, STATE_DECODE, 
      STATE_PTR_INC, STATE_PTR_DEC, STATE_VAL_INC, STATE_VAL_DEC,
      STATE_WHILE_START_1, STATE_WHILE_START_2, STATE_WHILE_START_3, STATE_WHILE_START_4, 
      STATE_WHILE_END_1, STATE_WHILE_END_2, STATE_WHILE_END_3, STATE_WHILE_END_4, STATE_WHILE_END_5,
      STATE_PUTCHAR, STATE_GETCHAR, STATE_BREAK, STATE_HALT
   );
   signal fsm_state_now : fsm_state_type;
   signal fsm_state_next : fsm_state_type;

 -- zde dopiste potrebne deklarace signalu

begin

   -- ---------- --
   --  Registry  --
   -- ---------- --

   -- PC 
   registr_pc: process (RESET, CLK, registr_pc_inc, registr_pc_dec)
   begin
      if RESET = '1' then -- asinchroni reset
         registr_pc_codeAddr <= ( others => '0' ); -- Vychozi hodnota adresy je nastavena na 0
      elsif (CLK'event and CLK='1') then
         -- Nesmi se provadet vice akci naraz - tj bud se inkrementuje nebo dekrementuje, ale ne oboje
         if registr_pc_inc = '1' then
            registr_pc_codeAddr <= registr_pc_codeAddr + 1;
         elsif registr_pc_dec = '1' then
            registr_pc_codeAddr <= registr_pc_codeAddr - 1;
         end if;

      end if; 
   end process;
   CODE_ADDR <= registr_pc_codeAddr;

   -- CNT
   registr_cnt: process (RESET, CLK, registr_cnt_inc, registr_cnt_dec)
   begin
      if RESET = '1' then -- asinchroni reset
         registr_cnt_value <= ( others => '0' );
      elsif (CLK'event and CLK = '1') then
         -- viz registr_pc
         if registr_cnt_inc = '1' then
            registr_cnt_value <= registr_cnt_value + 1;
         elsif registr_cnt_dec = '1' then
            registr_cnt_value <= registr_cnt_value - 1;
         end if;

      end if;
   end process;

   -- PTR
   registr_ptr: process (RESET, CLK, registr_ptr_inc, registr_ptr_dec)
   begin
      if RESET = '1' then -- asinchroni reset
         registr_ptr_dataAddr <= ( others => '0' ); -- Pocatecni adresa je nastavena na 0
      elsif (CLK'event and CLK = '1') then
         -- viz registr_pc
         if registr_ptr_inc = '1' then
            registr_ptr_dataAddr <= registr_ptr_dataAddr + 1;
         elsif registr_ptr_dec = '1' then
            registr_ptr_dataAddr <= registr_ptr_dataAddr - 1;
         end if;
      end if;
   end process;
   DATA_ADDR <= registr_ptr_dataAddr;

   -- ----------------------------- --
   --  Dekoder Brainlove instrukci  --
   -- ----------------------------- --
   decoder_brainlove_instruction: process (CODE_DATA)
   begin
      case CODE_DATA is
         when x"3e" => instruction <= ptr_inc;     -- >
         when x"3c" => instruction <= ptr_dec;     -- <
         when x"2b" => instruction <= val_inc;     -- +
         when x"2d" => instruction <= val_dec;     -- -
         when x"5b" => instruction <= while_start; -- [
         when x"5d" => instruction <= while_end;   -- ]
         when x"2e" => instruction <= putchar;     -- .
         when x"2c" => instruction <= getchar;     -- ,
         when x"7e" => instruction <= break;       -- ~
         when x"00" => instruction <= halt;        -- null
         when others=> instruction <= nop;         -- Neznamy vstup
      end case;
   end process;

   -- -------------------------------------- --
   --  Multiplexor pro vyber ukladanych dat  --
   -- -------------------------------------- --
   mx_wdata: process(sel_mx_wdata, DATA_RDATA, IN_DATA)
   begin
      case sel_mx_wdata is
         when "00" => DATA_WDATA <= IN_DATA;        -- 0 - Ukladaji se data ze vstupu
         when "01" => DATA_WDATA <= DATA_RDATA - 1; -- 1 - Ukladaji se data z pameti o jedna mensi
         when "10" => DATA_WDATA <= DATA_RDATA + 1; -- 2 - Ukladaji se data z pameti o jedna vetsi
         when others=> 
      end case;
   end process;

   -- --------------------- --
   --  FSM Konecny automat  --
   -- --------------------- --

   fsm_change_state_now: process(RESET, CLK, EN)
   begin
      if RESET = '1' then
         fsm_state_now <= STATE_FETCH;
      elsif (CLK'event and CLK = '1') then
         if EN = '1' then
            fsm_state_now <= fsm_state_next;
         end if;
      end if;
   end process;

   fsm_change_state_next: process(RESET, CLK, fsm_state_now, instruction, registr_cnt_value, IN_VLD, OUT_BUSY, DATA_RDATA )
   begin
      fsm_state_next <= STATE_FETCH;
      CODE_EN <= '1';   -- Povoleno cteni instrukce
      DATA_EN <= '0';   -- Zakazano cteni dat
      OUT_WE <= '0';    -- Zakazano vypisovani vystupu
      IN_REQ <= '0';    -- Zakazano cteni vstupu
      DATA_RDWR <= '0';

      -- Nastaveni vychozich hodnot pro ridici signaly registru
      registr_pc_inc  <= '0';  
      registr_pc_dec  <= '0';
      registr_cnt_inc <= '0';
      registr_cnt_dec <= '0';
      registr_ptr_inc <= '0';
      registr_ptr_dec <= '0';
      -- Vychozi volba multiplexoru
      sel_mx_wdata <= "00";
--

      case fsm_state_now is
         when STATE_HALT =>
            fsm_state_next <= STATE_HALT;
         when STATE_IDLE =>
            fsm_state_next <= STATE_FETCH;
            registr_pc_inc <= '1';

         when STATE_FETCH =>
            fsm_state_next <= STATE_DECODE;
            DATA_EN <= '1';
            DATA_RDWR <= '0';

         when STATE_DECODE => 
            case instruction is
               when ptr_inc =>
                  fsm_state_next <= STATE_PTR_INC;
               when ptr_dec =>
                  fsm_state_next <= STATE_PTR_DEC;
               when val_inc =>
                  fsm_state_next <= STATE_VAL_INC;
               when val_dec =>
                  fsm_state_next <= STATE_VAL_DEC;
               when while_start =>
                  fsm_state_next <= STATE_WHILE_START_1;
               when while_end =>
                  fsm_state_next <= STATE_WHILE_END_1;
               when putchar =>
                  fsm_state_next <= STATE_PUTCHAR;
               when getchar =>
                  fsm_state_next <= STATE_GETCHAR;
               when break =>
                  fsm_state_next <= STATE_BREAK;
               when halt =>
                  fsm_state_next <= STATE_HALT;
               when nop =>
                  fsm_state_next <= STATE_IDLE;
            end case;

         when STATE_PTR_INC => 
            registr_ptr_inc <= '1';
            registr_pc_inc <= '1';
            fsm_state_next <= STATE_FETCH;

         when STATE_PTR_DEC => 
            registr_ptr_dec <= '1';
            registr_pc_inc <= '1';
            fsm_state_next <= STATE_FETCH;

         when STATE_VAL_INC => 
            sel_mx_wdata <= "10";
            DATA_RDWR <= '1';
            DATA_EN <= '1';
            registr_pc_inc <= '1';
            fsm_state_next <= STATE_FETCH;

         when STATE_VAL_DEC => 
            sel_mx_wdata <= "01";
            DATA_RDWR <= '1';
            DATA_EN <= '1';
            registr_pc_inc <= '1';
            fsm_state_next <= STATE_FETCH;

         when STATE_PUTCHAR =>
            if OUT_BUSY = '1' then 
               fsm_state_next <= STATE_PUTCHAR;
            else
               DATA_RDWR <= '0';
               DATA_EN <= '1';
               OUT_WE <= '1';
               --OUT_DATA <= DATA_RDATA;
               registr_pc_inc <= '1';
               fsm_state_next <= STATE_FETCH;
            end if;

         when STATE_GETCHAR =>
            IN_REQ <= '1';
            if IN_VLD = '1' then
               sel_mx_wdata <= "00";
               DATA_RDWR <= '1';
               DATA_EN <= '1';
               registr_pc_inc <= '1';
               fsm_state_next <= STATE_FETCH;
            else
               fsm_state_next <= STATE_GETCHAR;
            end if;


         -- DATA_RDATA = ram[ptr]
         when STATE_WHILE_START_1 =>
            DATA_RDWR <= '0';
            DATA_EN <= '1';
            fsm_state_next <= STATE_WHILE_START_2;

         -- Podminka while( DATA_RDATA!=0 )
         when STATE_WHILE_START_2 =>
            registr_pc_inc <= '1';
            if DATA_RDATA = "00000000" then
               registr_cnt_inc <= '1';
               fsm_state_next <= STATE_WHILE_START_3;
            end if;

         when STATE_WHILE_START_3 =>
            fsm_state_next <= STATE_WHILE_START_4;

         -- Skok na konec cyklu
         when STATE_WHILE_START_4 =>
            if registr_cnt_value = "000000000000" then
               fsm_state_next <= STATE_FETCH;
            else
               registr_pc_inc <= '1';
               fsm_state_next <= STATE_WHILE_START_3;
               if instruction = while_start then
                  registr_cnt_inc <= '1';
               elsif instruction = while_end then
                  registr_cnt_dec <= '1';
               end if; 
            end if;

         -- DATA_RDATA = ram[ptr]
         when STATE_WHILE_END_1 =>
            DATA_RDWR <= '0';
            DATA_EN <= '1';
            fsm_state_next <= STATE_WHILE_END_2;

         -- if DATA_RDATA!=0
         when STATE_WHILE_END_2 =>
            if DATA_RDATA = "00000000" then
               registr_pc_inc <= '1';
            else
               registr_cnt_inc <= '1';
               registr_pc_dec <= '1';
               fsm_state_next <= STATE_WHILE_END_3;
            end if;

         when STATE_WHILE_END_3 =>
            fsm_state_next <= STATE_WHILE_END_4;

         -- Skok na zacatek cyklu
         when STATE_WHILE_END_4 =>
            if registr_cnt_value = "000000000000" then
               fsm_state_next <= STATE_FETCH;
            else
               if instruction = while_start then
                  registr_cnt_dec <= '1';
               elsif instruction = while_end then
                  registr_cnt_inc <= '1';
               end if; 
               fsm_state_next <= STATE_WHILE_END_5;
            end if;

         when STATE_WHILE_END_5 =>
            if registr_cnt_value = "000000000000" then
               registr_pc_inc <= '1';
            else
               registr_pc_dec <= '1';
            end if;
            fsm_state_next <= STATE_WHILE_END_3;

         when STATE_BREAK =>
            registr_pc_inc <= '1';
            registr_cnt_inc <= '1';
            fsm_state_next <= STATE_WHILE_START_3;

      end case;

   end process;

   OUT_DATA <= DATA_RDATA;
 -- zde dopiste vlastni VHDL kod

end behavioral;
 


