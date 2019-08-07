package main

import (
	"fmt"
	"reflect"
	"strings"

	core "k8s.io/api/core/v1"
)

func main() {
	pod := core.Pod{}
	podType := reflect.TypeOf(pod)

	g := NewGenerator()
	g.generateStruct(podType)

	for _, t := range g.TypeEnv {
		fmt.Printf("%s\n", t.Definition())
	}
}

type Generator struct {
	TypeEnv map[string]Type
}

type Type interface {
	impl() Type
	fmt.Stringer
	Definition() string
}

type TBool struct {
	Type
}

func (s *TBool) String() string {
	return "bool"
}

func (s *TBool) Definition() string {
	return ""
}

type TString struct {
	Type
}

func (s *TString) String() string {
	return "String"
}

func (s *TString) Definition() string {
	return ""
}

type TInt struct {
	Type

	Signed bool
	Width  uint
}

func (i *TInt) String() string {
	if !i.Signed {
		if i.Width == 0 {
			return "uint"
		} else {
			return fmt.Sprintf("u%d", i.Width)
		}
	} else {
		if i.Width == 0 {
			return "int"
		} else {
			return fmt.Sprintf("i%d", i.Width)
		}
	}
}

func (i *TInt) Definition() string {
	return ""
}

type TFloat struct {
	Type

	Width uint
}

func (f *TFloat) String() string {
	return fmt.Sprintf("f%d", f.Width)
}

func (f *TFloat) Definition() string {
	return ""
}

type TOption struct {
	Type

	TypeParameter Type
}

func (o *TOption) String() string {
	return "Option<" + o.TypeParameter.String() + ">"
}

type THashMap struct {
	Type

	TypeParameterKey   Type
	TypeParameterValue Type
}

func (h *THashMap) String() string {
	return "HashMap<" + h.TypeParameterKey.String() + ", " + h.TypeParameterValue.String() + ">"
}

type TVector struct {
	Type

	TypeParameter Type
}

func (v *TVector) String() string {
	return "Vec<" + v.TypeParameter.String() + ">"
}

type TStructure struct {
	Type

	Accessibility string
	Name          string
	Fields        []Field
}
type Field struct {
	Accessibility string
	Notation      string
	Name          string
	Type          Type
}

func (r *TStructure) String() string {
	return r.Name
}
func (r *TStructure) Definition() string {
	buf := strings.Builder{}
	if r.Accessibility != "" {
		buf.WriteString(r.Accessibility + " ")
	}
	buf.WriteString("struct " + r.Name + " {\n")
	for _, field := range r.Fields {
		buf.WriteString("    ")
		fieldFormatString := fmt.Sprintf("%s: %s,", field.Name, field.Type)
		if field.Accessibility != "" {
			fieldFormatString = fmt.Sprintf("%s %s", field.Accessibility, fieldFormatString)
		}
		if field.Notation != "" {
			fieldFormatString = fmt.Sprintf("%s\n    %s", field.Notation, fieldFormatString)
		}
		buf.WriteString(fieldFormatString)
		buf.WriteString("\n")
	}
	buf.WriteString("}")
	return buf.String()
}

func NewGenerator() *Generator {
	tenv := map[string]Type{
		"bool":    &TBool{},
		"string":  &TString{},
		"int":     &TInt{Signed: true},
		"int8":    &TInt{Signed: true, Width: 8},
		"int16":   &TInt{Signed: true, Width: 16},
		"int32":   &TInt{Signed: true, Width: 32},
		"int64":   &TInt{Signed: true, Width: 64},
		"uint":    &TInt{},
		"uint8":   &TInt{Width: 8},
		"uint16":  &TInt{Width: 16},
		"uint32":  &TInt{Width: 32},
		"uint64":  &TInt{Width: 64},
		"float32": &TFloat{Width: 32},
		"float64": &TFloat{Width: 64},
	}
	return &Generator{
		TypeEnv: tenv,
	}
}

func (g *Generator) generateStruct(typ reflect.Type) Type {
	t, ok := g.TypeEnv[typ.Name()]
	if ok {
		return t
	}
	switch typ.Kind() {
	case reflect.Ptr:
		return g.generateStruct(typ.Elem())
	case reflect.Map:
		return &THashMap{
			TypeParameterKey:   g.generateStruct(typ.Key()),
			TypeParameterValue: g.generateStruct(typ.Elem()),
		}
	case reflect.Slice:
		return &TVector{
			TypeParameter: g.generateStruct(typ.Elem()),
		}
	case reflect.Struct:
	default:
		t, ok := g.TypeEnv[typ.Name()]
		if ok {
			return t
		}
		t = &TStructure{
			Accessibility: "pub",
			Name:          typ.Name(),
			Fields:        make([]Field, 0),
		}
		g.TypeEnv[typ.Name()] = t
		return t
	}
	rs := &TStructure{
		Accessibility: "pub",
		Name:          typ.Name(),
		Fields:        make([]Field, 0),
	}
	g.TypeEnv[typ.Name()] = rs
	for i := 0; i < typ.NumField(); i++ {
		f := typ.Field(i)
		tag, ok := f.Tag.Lookup("json")
		decodedTag := strings.Split(tag, ",")
		var (
			name string
			attr string
		)
		if len(decodedTag) >= 1 {
			name = decodedTag[0]
		}
		if len(decodedTag) >= 2 {
			attr = decodedTag[1]
		}

		switch attr {
		case "omitempty":
			if ok {
				rs.Fields = append(rs.Fields, Field{
					Accessibility: "pub",
					Notation:      "",
					Name:          name,
					Type:          &TOption{TypeParameter: g.generateStruct(f.Type)},
				})
			}
		case "inline":
			name := f.Name
			if ok {
				rs.Fields = append(rs.Fields, Field{
					Accessibility: "pub",
					Notation:      "#[serde(flatten)]",
					Name:          name,
					Type:          g.generateStruct(f.Type),
				})
			}
		}
	}
	return rs
}
