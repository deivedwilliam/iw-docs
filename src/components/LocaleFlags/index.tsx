import React from 'react';
import Link from '@docusaurus/Link';
import {useLocation} from '@docusaurus/router';
import useBaseUrl from '@docusaurus/useBaseUrl';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import {useAlternatePageUtils} from '@docusaurus/theme-common/internal';
import styles from './styles.module.css';

// Arquivo SVG da bandeira por locale (emoji nao renderiza no Windows)
const FLAG_FILE: Record<string, string> = {
  'pt-BR': 'br',
  en: 'us',
  es: 'es',
};

type Props = {
  mobile?: boolean;
};

type FlagLinkProps = {
  locale: string;
  to: string;
  label: string;
  active: boolean;
  mobile?: boolean;
};

function FlagLink({locale, to, label, active, mobile}: FlagLinkProps): React.ReactNode {
  const src = useBaseUrl(`/img/flags/${FLAG_FILE[locale] ?? 'br'}.svg`);
  return (
    <Link
      to={to}
      autoAddBaseUrl={false}
      target="_self"
      className={`${styles.flag} ${active ? styles.active : ''}`}
      title={label}
      aria-label={label}
      aria-current={active ? 'true' : undefined}
    >
      <img className={styles.flagImg} src={src} alt={label} width={22} height={15} />
      {mobile && <span className={styles.mobileLabel}>{label}</span>}
    </Link>
  );
}

export default function LocaleFlags({mobile}: Props): React.ReactNode {
  const {
    i18n: {currentLocale, locales, localeConfigs},
  } = useDocusaurusContext();
  const alternate = useAlternatePageUtils();
  const {search, hash} = useLocation();

  if (locales.length <= 1) return null;

  return (
    <div className={mobile ? styles.flagsMobile : styles.flags}>
      {locales.map((locale) => (
        <FlagLink
          key={locale}
          locale={locale}
          to={`pathname://${alternate.createUrl({locale, fullyQualified: false})}${search}${hash}`}
          label={localeConfigs[locale]?.label ?? locale}
          active={locale === currentLocale}
          mobile={mobile}
        />
      ))}
    </div>
  );
}
