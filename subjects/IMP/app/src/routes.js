import HomePage from './pages/HomePage.vue';
import NotFoundPage from './pages/not-found.vue';

import TextUploadPage from './pages/TextUploadPage.vue';
import SelectAnimationPage from './pages/SelectAnimationPage.vue';
import MatrixDisplay from './pages/MatrixDisplay.vue';
import MatrixAnimation from './pages/MatrixAnimation.vue';

import PanelLeftPage from './pages/panel-left.vue';
import SettingsPage from './pages/Settings.vue';

export default [
  {
    path: '/',
    component: HomePage,
  },
  {
    path: '/text/',
    component: TextUploadPage,
  },
  {
  	path: '/list/',
	  component: SelectAnimationPage,
  },
  {
    path: '/live/',
    component: MatrixDisplay,
  },
  {
    path: '/settings/',
    component: SettingsPage,
  },
  {
    path: '/create/',
    component: MatrixAnimation,
  },
  {
    path: '/panel-left/',
    component: PanelLeftPage,
  },
  {
    path: '(.*)',
    component: NotFoundPage,
  },
];
