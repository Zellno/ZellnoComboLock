# Homologacao 1.0.0

## Compatibilidade RaG Hunting Cabin

Preservar as cabines normal e admin existentes com seus respectivos
`Zellno_CombinationLock4`:

1. em cada cabine fechada, confirmar que aparece `Set Password (Zellno)`;
2. confirmar que nao aparecem `Discar numero`, `Proximo digito` nem as Actions originais de abrir e fechar a porta;
3. configurar uma senha de quatro a seis algarismos em cada cabine;
4. abrir com `Open (Zellno)` e fechar com `Close (Zellno)`;
5. confirmar que janelas, luzes e demais interacoes da cabine continuam disponiveis;
6. reiniciar cliente e servidor e repetir abertura e fechamento;
7. com alicate, remover o cadeado de cada cabine e confirmar que o item cai intacto;
8. confirmar que as Actions originais da porta e do cadeado analogico retornam depois da retirada;
9. confirmar o mesmo resultado na cabine normal e na cabine admin;
10. iniciar sem `@RaG_Hunting_Cabin` e confirmar que o Zellno Combo Lock continua carregando.
11. confirmar que `Death CarLock`/`Detach CarLock` nao aparece ao mirar a cabine com o cadeado Zellno;
12. verificar que cliente e servidor nao registram `NULL pointer` em `ActionDetachCarLock`, `ActivateZellnoActionSet` ou `OnItemLocationChanged`.

## Compatibilidade RaG BaseBuilding

Com uma porta RaG nova e um `Zellno_CombinationLock4` novo:

1. anexe o cadeado sem configurar previamente os mostradores;
2. confirme que aparece `Set Password (Zellno)`;
3. confirme que nao aparecem `Dial number` nem `Next digit`;
4. configure uma senha entre quatro e seis algarismos;
5. confirme que nao aparecem `Abrir`/`Open` nem `Fechar`/`Close` originais;
6. abra com `Open (Zellno)` e feche com `Close (Zellno)`;
7. confirme que o cadeado permanece anexado;
8. com alicate, confirme `Remove Lock (Zellno)` e a soltura do item intacto;
9. confirme que as Actions originais da porta RaG reaparecem depois da retirada;
10. reinicie sem apagar a porta e valide novamente abertura, fechamento e persistencia;
11. execute uma inicializacao sem RaG BaseBuilding e confirme que o addon Zellno continua carregando.

O teste deve usar uma porta RaG nova. Uma estrutura da qual um attachment foi
apagado administrativamente pode manter estado inconsistente e nao serve para
homologar o fluxo.

## Regressoes preservadas da 0.4.3

## Pre-condicoes

- Servidor local de testes parado antes da instalacao.
- Backup do perfil e da storage da missao local.
- Cliente e servidor carregando `@ZellnoComboLock`.
- Nenhuma alteracao na hospedagem 4Netplayers.
- Portao e cadeado persistidos do ensaio anterior preservados para repetir exatamente o mesmo caso.

## Diagnostico confirmado na 0.3.8

1. O servidor aceitou a Action.
2. `Start`, `OnStartClient` e `OpenForLock` executaram no cliente.
3. `ShowScriptedMenu` chamou `Init`.
4. `CreateWidgets` criou o layout sem erro.
5. Todos os widgets foram vinculados e `OnShow` executou; a geometria solicitada foi `454,294 620x420` em uma tela `1528x1008`, mas o painel permaneceu invisivel.
6. A `0.3.7` confirmou raiz e janela visiveis, retorno correto de `ShowScriptedMenu` e destruicao imediata do menu ao fim de `OpenForLock`.
7. A causa e a ausencia de uma referencia forte: `s_ActiveMenu` precisa ser declarado com `ref` para possuir a instancia.
8. A `0.3.8` manteve o menu vivo e chegou a segunda etapa, mas os quatro asteriscos permaneceram visiveis e Backspace produziu um encerramento abrupto sem crash registrado.

## Diagnostico confirmado na 0.3.9

1. A primeira entrada foi armazenada apenas no cliente para a confirmacao.
2. O valor interno do campo foi limpo corretamente antes da segunda entrada.
3. A mascara visual `****` permaneceu desenhada apesar de o valor interno estar vazio.
4. O segundo clique sem digitar confirmou que o servidor nao recebeu uma senha fantasma.
5. A confirmacao duplicada foi removida por ser desnecessaria para este fluxo.

## Diagnostico confirmado na 0.3.10

1. A senha foi informada uma unica vez e aceita pelo servidor.
2. A janela fechou depois da resposta positiva.
3. `Open (Zellno)` abriu o portao para o criador autorizado.
4. O texto da instrucao e dos botoes ficou superdimensionado e cortado.
5. O prompt da Action permaneceu desenhado sobre a janela.

## Validacao do campo, fechamento e escala

1. Confirmar que aparece `Set Password (Zellno)` no portao existente.
2. Pressionar `F` uma unica vez.
3. Registrar se a janela aparece no centro da tela, sem repetir a Action.
4. Se nao aparecer, encerrar cliente e servidor e coletar os logs do ensaio.
5. Digitar tres numeros e confirmar a recusa local.
6. Confirmar que os numeros reais aparecem no campo, sem mascara de asteriscos.
7. Digitar um quarto, quinto e sexto numero e confirmar que todos sao aceitos.
8. Tentar digitar um setimo numero e confirmar que ele desaparece imediatamente.
9. Digitar letras, espacos e simbolos e confirmar que desaparecem imediatamente.
10. Usar Backspace no final e Delete com o cursor antes de um caractere; confirmar edicao normal sem fechar cliente ou menu.
11. Selecionar `CANCEL` e confirmar fechamento e restauracao do controle, HUD e barra rapida.
12. Reabrir, pressionar Escape e confirmar o mesmo fechamento seguro.
13. Reabrir, informar de quatro a seis numeros e selecionar `SET PASSWORD` uma unica vez.
14. Confirmar que a janela fecha depois da resposta positiva do servidor.
15. Confirmar que titulo, instrucao, campo e botoes cabem integralmente na janela central.
16. Confirmar que prompt da Action, HUD e barra rapida permanecem ocultos enquanto a janela esta aberta.

## Configuracao depois de anexar

1. Criar um novo `Zellno_CombinationLock4` pelo VPP.
2. Instalar em um `Fence` convertido em portao.
3. Nao configurar os mostradores enquanto estiver nas maos.
4. Confirmar que aparece `Set Password (Zellno)` no portao.
5. Selecionar a Action e confirmar que a janela aparece imediatamente.
6. Informar menos de quatro caracteres e confirmar a recusa local.
7. Informar letras e confirmar a recusa local.
8. Informar de quatro a seis numeros e selecionar `SET PASSWORD` uma unica vez.
9. Confirmar que a janela fecha e que o cadeado permanece anexado.
10. Selecionar `Open (Zellno)` e confirmar a abertura para o criador autorizado.

## Jogador memorizado

1. Fechar pela Action `Close (Zellno)`.
2. Confirmar que aparece `Open (Zellno)`.
3. Abrir sem exibir a janela e sem digitar novamente.
4. Fechar, sair do cliente e reiniciar o servidor de modo limpo.
5. Entrar novamente e confirmar que `Open (Zellno)` ainda abre imediatamente.

## Fechamento Zellno

1. Abrir um portao com Zellno Combo Lock configurado.
2. Confirmar que aparece apenas `Close (Zellno)`.
3. Confirmar que a Action vanilla localizada `Fechar`/`Close` nao aparece junto.
4. Executar `Close (Zellno)` e confirmar que o portao fecha normalmente.
5. Confirmar que o cadeado permanece anexado e que `Open (Zellno)` volta a aparecer.
6. Em outro portao sem Zellno Combo Lock, confirmar que o fechamento vanilla continua disponivel.

## Segundo jogador

1. Com o mesmo portao fechado, usar um jogador que nunca acertou aquela combinacao.
2. Confirmar que `Open (Zellno)` apresenta a janela de senha.
3. Confirmar que uma senha incorreta nao abre o portao.
4. Informar a senha correta.
5. Confirmar que o portao abre e esse segundo jogador passa a conseguir reabrir sem senha.

## Remocao com alicate

1. Manter o portao fechado e o Zellno Combo Lock configurado e anexado.
2. Sem item nas maos, confirmar que a Action de remocao nao aparece.
3. Segurar um alicate arruinado e confirmar que a Action de remocao nao pode ser executada.
4. Segurar um alicate utilizavel e apontar para a area de interacao do portao.
5. Confirmar que aparece `Remove Lock (Zellno)`.
6. Manter a Action durante tres segundos.
7. Confirmar que o portao permanece construido, fechado e sem dano novo.
8. Confirmar que o cadeado cai intacto junto ao portao e pode ser recolhido.
9. Confirmar que o alicate recebe somente o desgaste de desmontagem.
10. Instalar novamente o mesmo cadeado e confirmar que ele exige nova configuracao de senha.
11. Em ensaio separado, confirmar que um jogador nao autorizado nao consegue concluir a remocao de um cadeado configurado.

## Regressao

1. Confirmar que `CombinationLock4` vanilla continua com o comportamento vanilla, inclusive caindo ao destrancar.
2. Com alicate nas maos e mirando uma Fence com Zellno Combo Lock, confirmar que `Detach CarLock` nao aparece.
3. Em um veiculo com CarLock real e com as permissoes normais, confirmar que `Detach CarLock` continua aparecendo e funcionando.
4. Verificar o RPT do cliente e do servidor por erros contendo `ZellnoComboLock`, `SCRIPT` ou `NULL pointer`.

## Fora deste teste

- Compatibilidade com BaseBuildingPlus (BBP).
- Compatibilidade com controle/gamepad e teclado virtual.
- Localizacao por stringtable.
- Bloqueio progressivo de tentativas.
