----------------------------------------------------------------------
-- Copyright (c) 2004,2010 Shinya Honda (honda@ertl.jp)
--
-- proc_int.vhd
--
-- Core VHDL
--
----------------------------------------------------------------------
library IEEE;
use IEEE.std_logic_1164.all;

entity proc_int is
	port(
		clk         : in  std_logic;
		reset_n     : in  std_logic;
		chipselect  : in  std_logic;
		address     : in  std_logic;
		write       : in  std_logic;
		writedata   : in  std_logic_vector(31 downto 0);
		read        : in  std_logic;
		readdata    : out std_logic_vector(31 downto 0);
		byteenable  : in  std_logic_vector(3 downto 0);
		waitrequest : out std_logic;
		irq         : out std_logic
	);
end proc_int;

----------------------------------------------------------------------
-- Architecture section
----------------------------------------------------------------------
architecture rtl of proc_int is
	signal irq_reg : std_logic;

begin -- architecture rtl

	waitrequest <= '0';
	irq <= irq_reg;
	readdata(31 downto 1) <= (others=>'0');
	readdata(0) <= irq_reg;

	process(clk, reset_n)
	begin
		if ( reset_n = '0' ) then
			irq_reg <= '0';
		elsif( clk = '1' and clk'event ) then
			if ( write = '1' and chipselect = '1' ) then
				irq_reg <= writedata(0);
			else
				irq_reg <= irq_reg;
			end if;
		end if;
	end process;

end rtl;
