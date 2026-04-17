#!/bin/bash

echo "{
	libreadline
	Memcheck:Leak
	...
	obj:*libreadline.so.*
	...
}
{
   readline_add_history_suppression
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   fun:xmalloc
   fun:add_history
}
{
   readline_leak_fix
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   fun:getdelim
   fun:readline
}
{
   readline_realloc_suppression
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:realloc
   fun:getdelim
   fun:readline
}" > ~/42_minishell_tester/utils/minishell.supp

