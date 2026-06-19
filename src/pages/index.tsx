import React from 'react';
import Link from '@docusaurus/Link';
import Layout from '@theme/Layout';
import styles from './index.module.css';

export default function Home() {
  return (
    <Layout
      title="Documentação oficial da Indústrias William"
      description="Documentação oficial das placas, módulos e soluções da Indústrias William">
      <main className={styles.main}>
        <section className={styles.hero}>
          <div className={styles.heroContent}>
            <span className={styles.badge}>Documentação oficial</span>

            <h1>Documentação oficial da Indústrias William</h1>

            <p>
              Guias, imagens, exemplos, pinouts e referências para desenvolver
              com as plataformas da Indústrias William.
            </p>

            <div className={styles.buttons}>
              <Link className="button button--primary button--lg" to="/docs">
                Acessar documentação
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
              Visão geral dos produtos, recursos principais, hardware e
              posicionamento de cada plataforma.
            </p>
          </div>

          <div className={styles.card}>
            <h3>Firmware e integração</h3>
            <p>
              Orientações de ambiente, stacks suportadas e caminhos sugeridos
              para levar os projetos para a bancada.
            </p>
          </div>

          <div className={styles.card}>
            <h3>Exemplos práticos</h3>
            <p>
              Casos de uso reais para acelerar protótipos, validações internas
              e demonstrações técnicas.
            </p>
          </div>

          <div className={styles.card}>
            <h3>Software e expansões</h3>
            <p>
              Módulos complementares e ferramentas da casa para fechar o
              ecossistema de desenvolvimento.
            </p>
          </div>
        </section>
      </main>
    </Layout>
  );
}
