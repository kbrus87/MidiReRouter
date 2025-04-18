export { }; // Esto convierte el archivo en un módulo

declare global {
  interface Window {
    __JUCE__: JUCE;
  }
  interface MidiTableEventRow {
    id: number;
    inputMIDI: string;
    inputMIDInumber: number;
    outputMIDI: string;
    outputMIDInumber: number;
    active: boolean;
    inputFantasyName: string;
    outputFantasyName: string;
  }
}

interface JUCE {
  getAndroidUserScripts?: () => string;
  postMessage: (message: string) => void;
  initialisationData: InitialisationData;
  backend: Backend;
}

export interface drummapElement { midiNumber: number, fantasyName: string }

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


export const numberToName = new Map<number, string>([
  [-1, "C-2"], [0, "--"], [1, "C#-2"], [2, "D-2"], [3, "D#-2"], [4, "E-2"], [5, "F-2"],
  [6, "F#-2"], [7, "G-2"], [8, "G#-2"], [9, "A-2"], [10, "A#-2"], [11, "B-2"],
  [12, "C-1"], [13, "C#-1"], [14, "D-1"], [15, "D#-1"], [16, "E-1"], [17, "F-1"],
  [18, "F#-1"], [19, "G-1"], [20, "G#-1"], [21, "A-1"], [22, "A#-1"], [23, "B-1"],
  [24, "C0"], [25, "C#0"], [26, "D0"], [27, "D#0"], [28, "E0"], [29, "F0"],
  [30, "F#0"], [31, "G0"], [32, "G#0"], [33, "A0"], [34, "A#0"], [35, "B0"],
  [36, "C1"], [37, "C#1"], [38, "D1"], [39, "D#1"], [40, "E1"], [41, "F1"],
  [42, "F#1"], [43, "G1"], [44, "G#1"], [45, "A1"], [46, "A#1"], [47, "B1"],
  [48, "C2"], [49, "C#2"], [50, "D2"], [51, "D#2"], [52, "E2"], [53, "F2"],
  [54, "F#2"], [55, "G2"], [56, "G#2"], [57, "A2"], [58, "A#2"], [59, "B2"],
  [60, "C3"], [61, "C#3"], [62, "D3"], [63, "D#3"], [64, "E3"], [65, "F3"],
  [66, "F#3"], [67, "G3"], [68, "G#3"], [69, "A3"], [70, "A#3"], [71, "B3"],
  [72, "C4"], [73, "C#4"], [74, "D4"], [75, "D#4"], [76, "E4"], [77, "F4"],
  [78, "F#4"], [79, "G4"], [80, "G#4"], [81, "A4"], [82, "A#4"], [83, "B4"],
  [84, "C5"], [85, "C#5"], [86, "D5"], [87, "D#5"], [88, "E5"], [89, "F5"],
  [90, "F#5"], [91, "G5"], [92, "G#5"], [93, "A5"], [94, "A#5"], [95, "B5"],
  [96, "C6"], [97, "C#6"], [98, "D6"], [99, "D#6"], [100, "E6"], [101, "F6"],
  [102, "F#6"], [103, "G6"], [104, "G#6"], [105, "A6"], [106, "A#6"], [107, "B6"],
  [108, "C7"], [109, "C#7"], [110, "D7"], [111, "D#7"], [112, "E7"], [113, "F7"],
  [114, "F#7"], [115, "G7"], [116, "G#7"], [117, "A7"], [118, "A#7"], [119, "B7"],
  [120, "C8"], [121, "C#8"], [122, "D8"], [123, "D#8"], [124, "E8"], [125, "F8"],
  [126, "F#8"], [127, "G8"], [128, "G#8"], [129, "A8"], [130, "A#8"], [131, "B8"],
  [132, "C9"]
]);
