-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic 
	
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (PRINT_MESSAGE, FINISH, 
	TEST1, TEST15, TEST151, TEST1514, TEST15141, TEST151415,
	TEST1514157, TEST15141578, TEST151415788, TEST1514157884,
	TEST15141578848OK1, TEST15141578848OK2, -- Ukonceni pro prvni kod, a pro druhy kod
	TEST_BAD_WAY, -- Zadani spatneho cisla
	PRINT_OK, PRINT_FAILD -- Tiskuni vystupu
	);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(1) = '1') then
         next_state <= TEST15; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST15 =>
      next_state <= TEST15;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(5) = '1') then
         next_state <= TEST151; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST151 =>
      next_state <= TEST151;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(1) = '1') then
         next_state <= TEST1514; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1514 =>
      next_state <= TEST1514;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(4) = '1') then
         next_state <= TEST15141; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST15141 =>
      next_state <= TEST15141;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(1) = '1') then
         next_state <= TEST151415; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST151415 =>
      next_state <= TEST151415;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(5) = '1') then
         next_state <= TEST1514157; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1514157 =>
      next_state <= TEST1514157;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(7) = '1') then
         next_state <= TEST15141578; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST15141578 =>
      next_state <= TEST15141578;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(8) = '1') then
         next_state <= TEST151415788; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST151415788 =>
      next_state <= TEST151415788;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(8) = '1') then
         next_state <= TEST1514157884; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1514157884 =>
      next_state <= TEST1514157884;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
      if (KEY(4) = '1') then
         next_state <= TEST15141578848OK1; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST15141578848OK1 =>
      next_state <= TEST15141578848OK1;
      if (KEY(15) = '1') then
         next_state <= PRINT_OK; -- Konec prvniho kodu 
      end if;
      if (KEY(8) = '1') then
         next_state <= TEST15141578848OK2; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST15141578848OK2 =>
      next_state <= TEST15141578848OK2;
      if (KEY(15) = '1') then
         next_state <= PRINT_OK; -- Konec druheho kodu 
      end if;
		if (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD_WAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	-- Chybny kod
   when TEST_BAD_WAY =>
      next_state <= TEST_BAD_WAY;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAILD; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	-- VYPISY
   when PRINT_OK =>
      next_state <= PRINT_OK;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_FAILD =>
      next_state <= PRINT_FAILD;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- when TEST1 =>
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_OK =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM		<= '1'; -- Spravne heslo
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_FAILD =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM		<= '0'; -- Spatne heslo
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;

