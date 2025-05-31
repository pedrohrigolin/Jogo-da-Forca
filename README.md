# 🎮 Projeto Jogo da Forca — Planejamento e Estrutura

## ✅ Funcionalidades já implementadas

### 🔤 Strings
- ✅ Função de normalização de strings *(remove espaços, acentos e converte para maiúsculas)*

### 🧰 Utilitárias gerais
- ✅ Função que limpa a tela *(diferencia Linux e Windows)*

### 📂 Utilitárias de arquivo
- ✅ Verificação se o arquivo existe  
- ✅ Leitura de arquivo e retorno de conteúdo completo  
- ✅ Escrita de conteúdo em arquivo *(com sobreposição)*  
- ✅ Normalização do caminho do arquivo com base no SO  

### 📄 Criação de arquivos
- ✅ Criação de arquivos com segurança *(verifica permissão, erro, etc.)*

### 📖 Leitura de arquivos
- ✅ Extração completa e validação do conteúdo

### 🔍 Verificação
- ✅ Verificação se o conteúdo do arquivo está em conformidade *(somente letras, etc.)*

---

## 🚧 Funcionalidades em desenvolvimento / planejadas

### 🔐 Verificação e integridade
- 🔄 Finalizar função de verificação de integridade de arquivos *(palavras e usuários)*

### ⚙️ Inicialização
- 🔄 Função de inicialização do programa:
  - Verifica existência e integridade dos arquivos
  - Cria arquivos padrão se necessário
  - Lê e extrai palavras para memória (cache)
  - Lê e extrai usuários para memória (cache)

### 🧠 Cache
- 🔄 Sistema de cache de palavras em memória (RAM) por nível *(fácil, normal, difícil)*

### ✏️ Edição de palavras
- 🔄 Sistema de edição de palavras *(adicionar, excluir, substituir tudo)*  
- 🔄 Opção de definir conjunto manualmente ou automaticamente *(por tamanho da palavra)*

### 👤 Sistema de perfis
- 🔄 Cadastro, login, remoção  
- 🔄 Armazenamento de palavras personalizadas *(talvez)*

### 🧭 Menu
- 🔄 Sistema de interação: jogar, editar palavras, escolher perfil, nível, estatísticas, etc.

### 📦 Modularização
- 🔄 Separação do código em múltiplos arquivos `.cpp` e `.h`

### 📜 Regras
- 🔄 Criar documento com as regras do jogo

### 📊 Estatísticas *(opcional)*
- 🔄 Sistema de estatísticas por usuário *(partidas, acertos, erros)*

### 💡 Dicas *(opcional)*
- 🔄 Implementar sistema de dicas dependendo da dificuldade

### 🌀 Recursividade *(pensar em aplicações)*
- 🔄 Tentar aplicar alguma parte do fluxo com recursividade se possível

---

## 🧩 Fluxo geral do programa (descrição)

### 🚀 Ao iniciar o programa:
1. Chama a função de inicialização:
   - Verifica se os arquivos existem
   - Cria arquivos padrão, se necessário
   - Lê e extrai dados dos arquivos
   - Carrega dados na memória (palavras e usuários)
2. Se a inicialização falhar, o programa é encerrado.

### 📋 No menu inicial:
- Criar / apagar perfil  
- Escolher nível de dificuldade  
- Adicionar / excluir / substituir palavras  
- Definir palavra temporária  
- Jogar  
- Ver estatísticas *(talvez)*

### 🎲 Ao jogar:
- Se não houver palavra temporária, seleciona aleatória do cache  
- Usuário tenta acertar letras  
- Valida se a letra existe na palavra  
- Atualiza progresso, tentativas, etc.  
- Mostra boneco da forca  
- Talvez mostra dica  
- Ao fim, retorna ao menu

---

## 🗂️ Modularização planejada

| Arquivo / Módulo       | Responsabilidade                                 |
|------------------------|--------------------------------------------------|
| `main.cpp`             | Função principal, lógica do menu inicial         |
| `init.cpp` / `init.h`  | Inicialização, verificação de arquivos           |
| `strings.cpp` / `.h`   | Funções de normalização de strings               |
| `fileio.cpp` / `.h`    | Leitura e escrita de arquivos                    |
| `usuarios.cpp` / `.h`  | Manipulação dos perfis de usuários               |
| `palavras.cpp` / `.h`  | Cache, leitura e edição das palavras             |
| `jogo.cpp` / `.h`      | Lógica do jogo da forca                          |
| `menu.cpp` / `.h`      | Sistema de menu e opções                         |
| `utils.cpp` / `.h`     | Funções auxiliares gerais                        |

---

# 🧪 Rodando Código via Terminal (CMake + MinGW)

### 📁 **1. Criar e acessar a pasta `build/`**
```bash
mkdir build
cd build
```

---

### ⚙️ **2. Gerar arquivos com MinGW**
```bash
cmake .. -G "MinGW Makefiles"
```

---

### 🛠️ **3. Compilar o projeto**
```bash
cmake --build .
```

---

### ▶️ **4. Rodar o executável**
```bash
.\..bin\JogoDaForca.exe
```

---

## 🔄 Caso já tenha gerado arquivos CMake anteriormente...

### ❌ **Limpar cache antigo (Importante!)**
```powershell
Remove-Item -Recurse -Force CMakeCache.txt, CMakeFiles
```

> 💡 **Dica:** Sempre limpe o cache se mudar configurações do CMake ou trocar compilador.

---
