import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

const config: Config = {
  title: 'Industrias William Docs',
  tagline: 'Documentacao tecnica das placas e solucoes IW',
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
    locales: ['pt-BR'],
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
      title: 'Industrias William',
      logo: {
        alt: 'Industrias William Logo',
        src: 'img/logo.png',
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'tutorialSidebar',
          position: 'left',
          label: 'Documentacao',
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
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Documentacao',
          items: [
            {
              label: 'Inicio',
              to: '/docs',
            },
            {
              label: 'Devkits e placas',
              to: '/docs/devkits',
            },
            {
              label: 'Modulos e expansoes',
              to: '/docs/modules',
            },
            {
              label: 'Software',
              to: '/docs/software',
            },
          ],
        },
      ],
      copyright: `Copyright ${new Date().getFullYear()} Industrias William. Built with Docusaurus.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
