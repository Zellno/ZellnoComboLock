# Zellno Combo Lock

Implementacao independente para modernizar o uso do cadeado de combinacao de quatro digitos do DayZ.

## Estado atual

`1.0.0` preserva o nucleo independente e adiciona um segundo PBO opcional de compatibilidade. `optional/ZellnoComboLock_RaGCompat.pbo` declara dependencias explicitas de `ZellnoComboLock` e `RaG_BaseBuilding`, garantindo que seus filtros `4_World` sejam aplicados depois das alteracoes do RaG sem impedir o uso do PBO principal em servidores sem RaG. Servidores com RaG devem copiar esse PBO e sua assinatura da pasta `optional` para `addons`; servidores sem RaG devem deixa-los em `optional`.

O addon registra o classname:

`Zellno_CombinationLock4`

Ele herda diretamente de `CombinationLock4` e reutiliza apenas os recursos vanilla fornecidos pelo DayZ.

O nucleo homologado:

- memoriza no servidor o jogador que acertou a combinacao;
- abre o `Fence` sem remover o cadeado do slot;
- permite que o jogador autorizado volte a abrir com a Action `Open (Zellno)`;
- substitui a Action vanilla de fechamento por `Close (Zellno)` somente nos portoes com Zellno Combo Lock;
- persiste ate 64 IDs autorizados por cadeado;
- limpa as autorizacoes quando o cadeado e instalado novamente;
- preserva as autorizacoes depois de reiniciar servidor e cliente.
- permite ao jogador autorizado remover o cadeado intacto com um alicate.
- oculta o falso positivo `Detach CarLock` somente em cercas protegidas pelo Zellno Combo Lock.
- usa o fluxo eletronico Zellno nas portas RaG compativeis sem depender de classes ou recursos do RaG.
- oculta os mostradores analogicos reintroduzidos pelas portas RaG somente quando o item anexado e Zellno.

O incremento `0.4.1-pliers-action-registration-lab` registra a Action continua `Remove Lock (Zellno)` diretamente no alicate, como exigido pelo sistema de acoes com item principal do DayZ. Ela aparece quando o jogador aponta para o portao fechado segurando um alicate em condicao de uso. A autorizacao e revalidada no servidor ao concluir a Action. O comportamento vanilla destrava o slot e deposita o cadeado intacto junto ao portao; o portao nao e desmontado nem danificado. Cadeados ainda sem senha tambem podem ser recuperados.

## Compatibilidade com BaseBuildingPlus

Zellno Combo Lock ainda nao foi testado com BaseBuildingPlus (BBP). A compatibilidade com portas, portoes e outras estruturas bloqueaveis do BBP nao e garantida nesta versao. Administradores nao devem considerar BBP suportado antes de testes dedicados de compatibilidade.

## Escopo da versao 1.0.0

Comprovar que o novo item:

- aparece no VPPAdminTools pelo classname correto;
- usa o mesmo modelo e inventario do cadeado vanilla;
- pode ser levado nas maos;
- pode ser anexado ao slot `Att_CombinationLock` de um `Fence` vanilla;
- abre localmente a interface para configurar um cadeado novo;
- fecha a interface pelo botao CANCEL e pela tecla Escape;
- configura a nova senha com uma unica entrada valida;
- aceita de quatro a seis algarismos, inclusive combinacoes com zero inicial;
- rejeita a senha incorreta sem utilizar os mostradores;
- valida a combinacao correta no servidor sem soltar o cadeado;
- abre e fecha o portao mantendo o cadeado instalado e logicamente trancado;
- permite reabrir com `Open (Zellno)` para o jogador autorizado;
- permite fechar com `Close (Zellno)` sem duplicar a Action vanilla;
- preserva `Detach CarLock` em veiculos reais e o oculta na Fence Zellno;
- exige nova senha de um jogador ainda nao autorizado;
- preserva a autorizacao apos restart.
- oferece `Set Password (Zellno)`, `Open (Zellno)` e `Close (Zellno)` nas portas RaG compativeis;
- nao oferece as Actions analogicas de trocar mostrador ou discar numero nessas portas quando o lock e Zellno;
- nao oferece `Abrir` ou `Fechar` originais junto de `Open (Zellno)` e `Close (Zellno)`;
- restaura as Actions originais da porta RaG depois da retirada do cadeado Zellno;
- continua funcionando sem RaG BaseBuilding instalado ou carregado;
- oferece o fluxo Zellno na classe `rag_hunting_cabin`, tanto para a cabine normal quanto para a cabine admin;
- preserva janelas, luzes, construcao e demais Actions nao relacionadas a porta da cabine;
- continua funcionando sem RaG Hunting Cabin instalado ou carregado;
- oferece a remocao somente com alicate utilizavel;
- recusa a remocao de um cadeado configurado para jogador nao autorizado;
- solta o cadeado intacto junto ao portao sem destruir a estrutura;
- permite recolher e instalar novamente o mesmo item.

## Migracao deliberadamente segura

Objetos persistidos no formato anterior sao lidos, mas passam a estado sem senha Zellno e precisam ser configurados pela nova Action. Nenhuma combinacao analogica anterior e promovida automaticamente.

## Isolamento

O classname vanilla `CombinationLock4` nao e modificado. Esta versao nao contem codigo nem recursos de Code Lock, ZenComboLocks ou qualquer outro mod de terceiros.

## Build

O `build.sh` segue o fluxo Linux/Wine usado pelos outros projetos Zellno e opera apenas dentro de `build/@ZellnoComboLock`. Ele compila, assina e verifica o PBO principal e o PBO opcional de compatibilidade com RaG BaseBuilding, mas nao instala o addon no cliente nem no servidor.
