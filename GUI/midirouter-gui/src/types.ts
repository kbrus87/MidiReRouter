export {}; // Esto convierte el archivo en un módulo

declare global {
  interface Window {
    __JUCE__: JUCE;
  }
  interface MidiTableEventRow {
    inputMIDI: string;
    inputMIDInumber: number;
    outputMIDI: string;
    outputMIDInumber: number;
  }
}

interface JUCE {
  getAndroidUserScripts?: () => string;
  postMessage: (message: string) => void;
  initialisationData: InitialisationData;
  backend: Backend;
}

interface InitialisationData {
  __juce__platform: any[];
  __juce__functions: any[];
  __juce__registeredGlobalEventIds: any[];
  __juce__sliders: any[];
  __juce__toggles: any[];
  __juce__comboBoxes: any[];
}

class ListenerList {
  private listeners: Map<number, (payload: any) => void>;
  private listenerId: number;

  constructor() {
    this.listeners = new Map();
    this.listenerId = 0;
  }

  addListener(fn: (payload: any) => void): number {
    const newListenerId = this.listenerId++;
    this.listeners.set(newListenerId, fn);
    return newListenerId;
  }

  removeListener(id: number): void {
    this.listeners.delete(id);
  }

  callListeners(payload: any): void {
    this.listeners.forEach((listener) => listener(payload));
  }
}

class EventListenerList {
  private eventListeners: Map<string, ListenerList>;

  constructor() {
    this.eventListeners = new Map();
  }

  addEventListener(
    eventId: string,
    fn: (payload: any) => void
  ): [string, number] {
    if (!this.eventListeners.has(eventId)) {
      this.eventListeners.set(eventId, new ListenerList());
    }
    const id = this.eventListeners.get(eventId)!.addListener(fn);
    return [eventId, id];
  }

  removeEventListener([eventId, id]: [string, number]): void {
    if (this.eventListeners.has(eventId)) {
      this.eventListeners.get(eventId)!.removeListener(id);
    }
  }

  emitEvent(eventId: string, object: any): void {
    if (this.eventListeners.has(eventId)) {
      this.eventListeners.get(eventId)!.callListeners(object);
    }
  }
}

class Backend {
  private listeners: EventListenerList;

  constructor() {
    this.listeners = new EventListenerList();
  }

  addEventListener(
    eventId: string,
    fn: (payload: any) => void
  ): [string, number] {
    return this.listeners.addEventListener(eventId, fn);
  }

  removeEventListener([eventId, id]: [string, number]): void {
    this.listeners.removeEventListener([eventId, id]);
  }

  emitEvent(eventId: string, object: any): void {
    window.__JUCE__.postMessage(JSON.stringify({ eventId, payload: object }));
  }

  emitByBackend(eventId: string, object: string): void {
    this.listeners.emitEvent(eventId, JSON.parse(object));
  }
}


