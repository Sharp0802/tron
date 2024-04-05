using System.Collections;
using Tron.Runtime.CodeGen;
using Tron.Runtime.InteropServices;
using Tron.Runtime.Reflection;
using Transform = Tron.Runtime.Specials.Transform;

namespace Tron.Runtime;

public abstract class Actor : CObject
{
    protected Actor()
    {
        unsafe
        {
            Pointer = NativeMemory.Alloc<CodeGen.Actor>();
            CodeGen.Actor.__ctor__(Pointer, new CObjectEvents(this));
        }
        Components = new ComponentCollection(this);
        Components.Add<Transform>();
    }
    
    internal unsafe CodeGen.Actor* Pointer { get; }

    public ComponentCollection Components { get; }
    
    public class ComponentCollection : ICollection<Component>
    {
        private readonly List<Component> _list  = [];
        private readonly Actor           _actor;

        internal ComponentCollection(Actor actor)
        {
            _actor = actor;
        }

        public int  Count      => _list.Count;
        public bool IsReadOnly => false;
        
        public T Add<T>() where T : Component
        {
            unsafe
            {
                var component = (T)Dynamic.GetCtor(typeof(T), typeof(Actor))(_actor);
                CodeGen.Actor.AddComponent(_actor.Pointer, component.Pointer);
                _list.Add(component);
                return component;
            }
        }

        void ICollection<Component>.Add(Component item)
        {
            throw new NotSupportedException();
        }

        public bool Remove(Component component)
        {
            // TODO: Handle special components
            unsafe
            {
                var ret = CodeGen.Actor.RemoveComponent(_actor.Pointer, component.Pointer);
                if (!ret)
                    return false;
                ret = _list.Remove(component);
                return ret;
            }
        }

        public void Clear()
        {
            // TODO: Handle special components
            foreach (var component in _list) 
                Remove(component);
        }

        public bool Contains(Component item)
        {
            return _list.Contains(item);
        }

        public void CopyTo(Component[] array, int arrayIndex)
        {
            _list.CopyTo(array, arrayIndex);
        }

        public IEnumerator<Component> GetEnumerator() => _list.GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }
}