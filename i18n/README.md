# Internacionalização (i18n)

O site está configurado para três idiomas:

- `pt-BR` — Português (Brasil) — **idioma padrão** (conteúdo-fonte em `docs/`, `blog/`, `src/pages/`)
- `en` — English
- `es` — Español

O seletor de idioma aparece no canto direito da barra de navegação.

## Como o conteúdo é resolvido

Para `en` e `es`, qualquer página **ainda não traduzida** usa automaticamente a versão em `pt-BR` (fallback). Ou seja, o site já funciona nos três idiomas; a tradução pode ser feita de forma incremental.

## Onde ficam as traduções

```
i18n/
  en/
    code.json                         # textos da UI do tema (Next/Previous, etc.)
    docusaurus-theme-classic/
      navbar.json                     # rótulos da navbar
      footer.json                     # rótulos do rodapé
    docusaurus-plugin-content-docs/
      current/                        # cópias traduzidas dos .md de docs/ (criar conforme traduzir)
  es/
    ... (mesma estrutura)
```

## Passo a passo para traduzir

1. Gerar/atualizar os arquivos de tradução da UI:

   ```bash
   npm run write-translations -- --locale en
   npm run write-translations -- --locale es
   ```

   Isso completa `code.json`, `navbar.json` e `footer.json` com todas as chaves atuais.

2. Traduzir o conteúdo dos docs: copie o arquivo de `docs/...` para
   `i18n/<locale>/docusaurus-plugin-content-docs/current/...` (mesmo caminho relativo) e traduza o texto.
   Não altere `id`, `slug`, nomes de arquivos `.glb`/imagens nem os blocos `<BoardViewer .../>`.

3. Rodar localmente em um idioma específico:

   ```bash
   npm run start -- --locale en
   ```

4. Build de produção (gera todos os idiomas):

   ```bash
   npm run build
   ```

## Observação

Os arquivos `navbar.json`/`footer.json` aqui já trazem uma tradução inicial dos rótulos. Ao rodar `write-translations`, as chaves existentes são preservadas e as faltantes são adicionadas.
