import React from 'react';
import Link from '@docusaurus/Link';
import Layout from '@theme/Layout';
import styles from './index.module.css';

export default function Home() {
  return (
    <Layout
      title="Industrias William Docs"
      description="Documentacao oficial das placas, modulos e solucoes da Industrias William">
      <main className={styles.main}>
        <section className={styles.hero}>
          <div className={styles.heroContent}>
            <span className={styles.badge}>Documentacao oficial</span>

            <h1>Portfolio tecnico das placas IW</h1>

            <p>
              Guias, imagens, exemplos, pinouts e referencias para desenvolver
              com as plataformas da Industrias William.
            </p>

            <div className={styles.buttons}>
              <Link className="button button--primary button--lg" to="/docs">
                Acessar documentacao
              </Link>

              <Link
                className="button button--secondary button--lg"
                to="/docs/devkits">
                Explorar placas
              </Link>
            </div>
          </div>
        </section>

        <section className={styles.cards}>
          <div className={styles.card}>
            <h3>Devkits e placas</h3>
            <p>
              Visao geral dos produtos, recursos principais, hardware e
              posicionamento de cada plataforma.
            </p>
          </div>

          <div className={styles.card}>
            <h3>Firmware e integracao</h3>
            <p>
              Orientacoes de ambiente, stacks suportadas e caminhos sugeridos
              para levar os projetos para a bancada.
            </p>
          </div>

          <div className={styles.card}>
            <h3>Exemplos praticos</h3>
            <p>
              Casos de uso reais para acelerar prototipos, validacoes internas
              e demonstracoes tecnicas.
            </p>
          </div>

          <div className={styles.card}>
            <h3>Software e expansoes</h3>
            <p>
              Modulos complementares e ferramentas da casa para fechar o
              ecossistema de desenvolvimento.
            </p>
          </div>
        </section>
      </main>
    </Layout>
  );
}
