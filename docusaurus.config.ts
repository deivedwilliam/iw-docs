import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

const config: Config = {
  title: 'Indústrias William Docs',
  tagline: 'Documentação técnica das placas e soluções IW',
  favicon: 'img/favicon.ico',

  future: {
    v4: true,
  },

  url: 'https://your-docusaurus-site.example.com',
  baseUrl: '/',

  organizationName: 'IndustriasWilliam',
  projectName: 'iw-docs',

  onBrokenLinks: 'throw',

  i18n: {
    defaultLocale: 'pt-BR',
    locales: ['pt-BR', 'en', 'es'],
    localeConfigs: {
      'pt-BR': {label: 'Português', htmlLang: 'pt-BR', direction: 'ltr'},
      en: {label: 'English', htmlLang: 'en', direction: 'ltr'},
      es: {label: 'Español', htmlLang: 'es', direction: 'ltr'},
    },
  },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          exclude: ['**/Examples/**'],
        },
        blog: {
          showReadingTime: true,
          feedOptions: {
            type: ['rss', 'atom'],
            xslt: true,
          },
          onInlineTags: 'warn',
          onInlineAuthors: 'warn',
          onUntruncatedBlogPosts: 'warn',
        },
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    image: 'img/logo.png',
    colorMode: {
      respectPrefersColorScheme: true,
    },
    navbar: {
      title: 'Indústrias William',
      logo: {
        alt: 'Indústrias William Logo',
        src: 'img/logo.png',
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'tutorialSidebar',
          position: 'left',
          label: 'Documentação',
        },
        {
          to: '/docs/devkits',
          label: 'Placas',
          position: 'left',
        },
        {
          to: '/docs/software',
          label: 'Software',
          position: 'left',
        },
        {
          type: 'custom-localeFlags',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Documentação',
          items: [
            {
              label: 'Início',
              to: '/docs',
            },
            {
              label: 'Devkits e placas',
              to: '/docs/devkits',
            },
            {
              label: 'Módulos e expansões',
              to: '/docs/modules',
            },
            {
              label: 'Software',
              to: '/docs/software',
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Indústrias William. Built with Docusaurus.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
