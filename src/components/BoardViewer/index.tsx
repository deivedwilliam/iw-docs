import React, {useEffect, useRef, useState} from 'react';
import useBaseUrl from '@docusaurus/useBaseUrl';
import styles from './styles.module.css';

type BoardViewerProps = {
  /** Caminho do modelo .glb/.gltf (ex.: /models/iw-aiot-explorer-3d.glb) */
  src: string;
  /** Altura do visualizador em pixels */
  height?: number;
  /** Cor de fundo CSS ou "transparent" */
  background?: string;
  /** Gira o modelo automaticamente ate o usuario interagir */
  autoRotate?: boolean;
  /** Legenda exibida abaixo do visualizador */
  caption?: string;
};

type Status = 'loading' | 'ready' | 'error';

export default function BoardViewer({
  src,
  height = 480,
  background = 'transparent',
  autoRotate = true,
  caption,
}: BoardViewerProps): React.ReactElement {
  const containerRef = useRef<HTMLDivElement>(null);
  const [status, setStatus] = useState<Status>('loading');
  const [message, setMessage] = useState<string>('Carregando modelo 3D...');
  const modelUrl = useBaseUrl(src);

  useEffect(() => {
    const container = containerRef.current;
    if (!container) return;

    let disposed = false;
    let frameId = 0;
    let renderer: any;
    let controls: any;
    let resizeObserver: ResizeObserver | undefined;
    let userInteracted = false;

    (async () => {
      try {
        const THREE: any = await import('three');
        const {GLTFLoader}: any = await import(
          'three/examples/jsm/loaders/GLTFLoader.js'
        );
        const {OrbitControls}: any = await import(
          'three/examples/jsm/controls/OrbitControls.js'
        );
        if (disposed) return;

        const width = container.clientWidth || 600;

        const scene = new THREE.Scene();
        if (background !== 'transparent') {
          scene.background = new THREE.Color(background);
        }

        const camera = new THREE.PerspectiveCamera(45, width / height, 0.1, 100000);

        renderer = new THREE.WebGLRenderer({
          antialias: true,
          alpha: background === 'transparent',
        });
        renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
        renderer.setSize(width, height);
        if ('outputColorSpace' in renderer) {
          renderer.outputColorSpace = THREE.SRGBColorSpace;
        }
        container.appendChild(renderer.domElement);

        // Iluminacao
        scene.add(new THREE.AmbientLight(0xffffff, 0.55));
        const hemi = new THREE.HemisphereLight(0xffffff, 0x404048, 1.0);
        hemi.position.set(0, 1, 0);
        scene.add(hemi);
        const key = new THREE.DirectionalLight(0xffffff, 1.25);
        key.position.set(1, 1.2, 2);
        scene.add(key);
        const fill = new THREE.DirectionalLight(0xffffff, 0.65);
        fill.position.set(-1.5, -0.5, -1);
        scene.add(fill);

        controls = new OrbitControls(camera, renderer.domElement);
        controls.enableDamping = true;
        controls.dampingFactor = 0.08;
        controls.autoRotate = autoRotate;
        controls.autoRotateSpeed = 1.4;
        controls.addEventListener('start', () => {
          userInteracted = true;
          controls.autoRotate = false;
        });

        const loader = new GLTFLoader();
        loader.load(
          modelUrl,
          (gltf: any) => {
            if (disposed) return;
            const model = gltf.scene;

            // Centraliza o modelo na origem e enquadra a camera
            const box = new THREE.Box3().setFromObject(model);
            const size = box.getSize(new THREE.Vector3());
            const center = box.getCenter(new THREE.Vector3());
            model.position.sub(center);
            scene.add(model);

            const maxDim = Math.max(size.x, size.y, size.z) || 1;
            const fov = (camera.fov * Math.PI) / 180;
            let dist = maxDim / 2 / Math.tan(fov / 2);
            dist *= 1.7;

            camera.position.set(dist * 0.55, dist * 0.45, dist);
            camera.near = maxDim / 100;
            camera.far = maxDim * 100;
            camera.updateProjectionMatrix();
            controls.target.set(0, 0, 0);
            controls.update();

            setStatus('ready');
          },
          undefined,
          (err: unknown) => {
            // eslint-disable-next-line no-console
            console.error('[BoardViewer] erro ao carregar', src, err);
            setMessage('Falha ao carregar o modelo 3D.');
            setStatus('error');
          },
        );

        const animate = () => {
          frameId = requestAnimationFrame(animate);
          if (!userInteracted) controls.autoRotate = autoRotate;
          controls.update();
          renderer.render(scene, camera);
        };
        animate();

        resizeObserver = new ResizeObserver(() => {
          const w = container.clientWidth || width;
          camera.aspect = w / height;
          camera.updateProjectionMatrix();
          renderer.setSize(w, height);
        });
        resizeObserver.observe(container);
      } catch (err) {
        // eslint-disable-next-line no-console
        console.error('[BoardViewer] Three.js indisponivel', err);
        setMessage('Three.js nao instalado. Rode: npm install three');
        setStatus('error');
      }
    })();

    return () => {
      disposed = true;
      if (frameId) cancelAnimationFrame(frameId);
      resizeObserver?.disconnect();
      controls?.dispose?.();
      if (renderer) {
        renderer.dispose?.();
        const el = renderer.domElement;
        if (el && el.parentNode) el.parentNode.removeChild(el);
      }
    };
  }, [modelUrl, src, height, background, autoRotate]);

  return (
    <figure className={styles.viewer}>
      <div ref={containerRef} className={styles.canvas} style={{height}}>
        {status !== 'ready' && (
          <div className={styles.overlay} data-status={status}>
            {message}
          </div>
        )}
      </div>
      {caption && <figcaption className={styles.caption}>{caption}</figcaption>}
    </figure>
  );
}
