# Arduino Task Scheduler NTP
### (taskscheduler.ino)
#### Configurações de Agendador de Tarefas com gravação de dados na EEPROM

Este sketch exibe uma página WEB para configuração de até 10 tarefas para o arduino e também a pagina de configuração do endereço IP do NTP a ser usado bem como a TimeZone.

Os dados ficam gravados na EEPROM do Arduino e não se perdem ao desligar a placa.

para configurar a tarefa basta preencher o formulário na página /schdl
o sistema prioriza os dias da semana; portanto, se a data e os dias da semana forem preenchidos, a data será desconsiderada.

baseado no projeto https://github.com/brunosacom/ethernetipconfigeeprom
