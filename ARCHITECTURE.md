# Arquitetura 1.0.0

## Dependencias

- `DZ_Gear_Camping`: fornece `CombinationLock4` e seus recursos fisicos vanilla.
- `DZ_Scripts`: fornece a implementacao de script vanilla do cadeado.

## Classe

`Zellno_CombinationLock4 : CombinationLock4`

Somente `scope`, nome e descricao sao declarados. Modelo, texturas, materiais, slots, peso, tamanho, dano e comportamento permanecem herdados.

## Nucleo de autorizacao

- A lista de jogadores autorizados pertence ao proprio `Zellno_CombinationLock4`.
- O identificador persistido e `PlayerIdentity.GetId()` e nunca e sincronizado ao cliente.
- A senha correta enviada pela interface autoriza o jogador e abre o `Fence` sem liberar o slot nem derrubar o cadeado.
- A Action Zellno aparece no portao fechado com nosso cadeado; o servidor somente abre para um jogador autorizado.
- O fechamento protegido usa `ActionZellnoCloseFence`.
- Cada nova instalacao fisica do cadeado limpa a senha Zellno e a lista de autorizacoes.
- O limite e de 64 jogadores por cadeado.

## Fluxo eletronico, menu e remocao

### Compatibilidade opcional com RaG BaseBuilding

As portas RaG derivam de `Fence`, mas registram novamente as Actions analogicas
no proprio `SetActions()` e usam selecoes de modelo diferentes de
`gate_interact`. A compatibilidade Zellno trabalha somente com a interface
vanilla herdada (`Fence`, `GetCombinationLock`, `OpenFence` e `CloseFence`).

Nenhuma classe RaG aparece como tipo compilado no nucleo e `RaG_BaseBuilding`
nao foi adicionado a `requiredAddons[]` do PBO principal. A deteccao ocorre em
tempo de execucao pelo prefixo de classname `RaG_BB_`; assim o nucleo Zellno
permanece independente quando o RaG nao estiver instalado. O segundo PBO e
produzido em `optional/` e so deve ser movido para `addons/` em servidores que
usam RaG BaseBuilding.

Quando um cadeado Zellno esta anexado, as condicoes das Actions analogicas e das
Actions originais de abrir e fechar retornam `false` somente para aquela porta.
Ao retirar o cadeado, as condicoes originais voltam a ser atendidas naturalmente.
Locks vanilla e demais estruturas nao sao alterados.

### Compatibilidade opcional com RaG Hunting Cabin

O RaG Hunting Cabin publica o define `RAG_HUNTING_CABIN`. Todo o adaptador fica
protegido por esse define; sem o mod instalado, nenhuma classe RaG e compilada
ou exigida pelo Zellno Combo Lock.

As cabines normal e admin sao instancias da mesma classe `rag_hunting_cabin`.
A variante admin difere apenas pelo estado interno `m_IsAdminHuntingCabin`, por
isso um unico adaptador atende ambas. O adaptador usa o slot vanilla
`Att_CombinationLock`, a selecao `cabin_door` e os metodos publicos
`GetCombinationLock`, `OpenHuntCabDoor` e `CloseHuntCabDoor`.

Enquanto um Zellno Combo Lock estiver anexado, sao suprimidas somente
`ActionDialCombinationLockOnHuntingCabin`,
`ActionNextCombinationLockDialOnHuntingCabin`, `ActionOpenHuntingCabin` e
`ActionCloseHuntingCabin`. Janelas, luzes, construcao e demais acoes permanecem
intactas. Ao retirar o cadeado, as quatro acoes originais sao restauradas.

- A Action abre a interface no lado cliente em `Start`; abrir a UI nao depende de RPC.
- O servidor revalida jogador, identidade, estrutura fechada, cadeado anexado, estado trancado e distancia. Fence usa limite de 3 metros; RaG Hunting Cabin usa 5 metros porque seus attachments compartilham a origem central da cabine.
- Para jogador autorizado, o servidor abre o portao diretamente.
- A interface envia uma string de quatro a seis caracteres pelo jogador; ela nunca recebe a senha correta.
- Sem senha Zellno, o servidor grava a unica entrada valida e autoriza o criador.
- Com senha Zellno, o servidor compara, autoriza e abre somente em caso de igualdade.
- `m_ZellnoCode` existe somente no servidor; apenas `m_ZellnoConfigured` e sincronizado.
- As Actions analogicas herdadas sao removidas e `UnlockServer` nao aceita os discos vanilla.
- Uma tentativa nao altera `m_Combination`, os mostradores ou o slot do cadeado.
- Ha um intervalo minimo simples de 1 segundo por jogador e por cadeado. Ele protege o endpoint RPC sem constituir ainda um sistema progressivo de bloqueio.
- A raiz e o fundo recebem a resolucao atual em pixels; a janela de `500x320` e centralizada explicitamente antes e depois de `super.OnShow()`.
- A raiz remove `IGNOREPOINTER` e recebe `VISIBLE` e `RENDER_ALWAYS`.
- `s_ActiveMenu` e uma referencia forte e preserva a instancia criada por `ShowScriptedMenu` ate seu fechamento normal.
- Os componentes internos usam posicoes e tamanhos exatos; a configuracao envia uma unica entrada de quatro a seis numeros.
- Titulo, instrucao e status usam tamanho textual exato; botoes usam proporcao textual limitada em relacao a altura.
- O menu usa a API vanilla `Hud` para ocultar HUD e barra rapida durante a interacao e restaura ambos ao fechar.
- Backspace e Delete permanecem nativos do campo de edicao.
- `KC_ESCAPE` continua sendo tratado quando chega ao menu, mas o campo com foco pode consumir esse evento bruto.
- O `Update()` consulta `UAUIBack`, seguindo o padrao dos menus vanilla, e usa a mesma rotina idempotente do botao `CANCEL`.
- A restauracao de controles, HUD e barra rapida e idempotente e tambem executa em `Cleanup` como contingencia.
- A senha fica visivel durante a digitacao para permitir conferencia e compartilhamento deliberado pelo jogador.
- `Update` normaliza continuamente o campo porque `PasswordEditBoxWidget.OnChange` nao e emitido a cada tecla nesta interface.
- O cliente remove qualquer caractere nao numerico e corta o texto depois do sexto algarismo; o servidor repete a validacao antes de gravar ou comparar.

## Remocao com alicate

- Como a remocao usa um item principal, `ActionZellnoRemoveLock` e adicionada em `Pliers.SetActions()`; o registro apenas no alvo `Fence` nao e suficiente para a descoberta da Action.

- A Action exige `Pliers` nao arruinado, portao fechado, dobradicas e o `Zellno_CombinationLock4` anexado e trancado.
- O cliente pode apresentar a Action, mas nao recebe a lista privada de jogadores autorizados.
- Ao concluir os tres segundos, o servidor repete todas as validacoes e somente aceita jogador autorizado.
- Enquanto ainda nao existe senha Zellno, o cadeado pode ser recuperado para evitar aprisionamento acidental da estrutura.
- A remocao chama explicitamente o comportamento vanilla de `CombinationLock.UnlockServer`, que destrava o slot e solta o item intacto junto ao portao.
- O alicate recebe o desgaste vanilla de desmontagem somente quando a remocao realmente termina.

## Fechamento Zellno

- `ActionZellnoCloseFence` replica o fechamento imediato do portao no servidor e usa o texto `Close (Zellno)`.
- A Action exige um `Zellno_CombinationLock4` anexado e uma selecao de interacao valida para a estrutura.
- `ActionCloseFence.ActionCondition()` e interceptada somente para alvos com Zellno Combo Lock, impedindo a duplicacao da Action vanilla.
- Portoes sem Zellno Combo Lock continuam usando integralmente a Action vanilla.

## Compatibilidade opcional com TraderPlus CarLock

- O patch e protegido por `#ifdef CLDefine`, publicado pelo ecossistema CarLock do TraderPlus.
- `ActionDetachCarLock.ActionCondition()` retorna `false` somente quando o alvo resolve para um `Zellno_CombinationLock4`, uma `Fence` protegida ou uma RaG Hunting Cabin protegida. Alvo direto, pai e pai hierarquico sao verificados para cobrir os formatos entregues pelo sistema de Actions.
- Para veiculos e qualquer outro alvo, a implementacao original do TraderPlus e chamada por `super`.
- Nenhum PBO do TraderPlus e modificado e o Zellno Combo Lock continua utilizavel sem TraderPlus.
- Ao instalar novamente o cadeado, a senha e as autorizacoes anteriores sao apagadas pelo fluxo ja existente de nova instalacao.

## Fora deste incremento

- Bloqueio progressivo e configuravel de tentativas.
- Desafio descartavel por tentativa.
- Stringtable e traducoes.

Essas camadas serao adicionadas somente depois da homologacao pratica do nucleo no `Fence` vanilla.
