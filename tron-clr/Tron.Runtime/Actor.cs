using System.Collections;
using Tron.Runtime.CodeGen;
using Tron.Runtime.InteropServices;
using Tron.Runtime.Reflection;
using Transform = Tron.Runtime.Specials.Transform;

namespace Tron.Runtime;

/// <summary>
/// An object of engine that acts as an <see cref="Actor"/> in Actor-Model.
/// Hierarchy between <see cref="Actor"/>s forms with <see cref="Transform"/> <see cref="Component"/>.
/// </summary>
public abstract class Actor : CObject
{
    /// <summary>
    /// Initialize <see cref="Actor"/> on engine.
    /// </summary>
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

    ~Actor()
    {
        unsafe
        {
            NativeMemory.Free(Pointer);
        }
    }
    
    internal unsafe CodeGen.Actor* Pointer { get; }

    /// <summary>
    /// Gets <see cref="Component"/>s of the <see cref="Actor"/>.
    /// </summary>
    public ComponentCollection Components { get; }
    
    /// <summary>
    /// A collection of <see cref="Component"/>s bound to specific <see cref="Actor"/>.
    /// </summary>
    public class ComponentCollection : ICollection<Component>
    {
        private readonly List<Component> _list  = [];
        private readonly Actor           _actor;

        internal ComponentCollection(Actor actor)
        {
            _actor = actor;
        }

        /// <summary>
        /// Gets count of <see cref="Component"/>s
        /// </summary>
        public int  Count      => _list.Count;
        
        bool ICollection<Component>.IsReadOnly => false;
        
        /// <summary>
        /// Initializes <see cref="Component"/> and Binds it to <see cref="Actor"/>.
        /// </summary>
        /// <typeparam name="T">The type of <see cref="Component"/> to add</typeparam>
        /// <returns>Initialized <see cref="Component"/></returns>
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

        /// <summary>
        /// Removes registered <see cref="Component"/>.
        /// </summary>
        /// <param name="component">The <see cref="Component"/> to remove</param>
        /// <returns>False if the <see cref="Component"/> is not in <see cref="Actor"/>; otherwise, true</returns>
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

        /// <summary>
        /// Clears all registered <see cref="Component"/>s except <see cref="Transform"/>.
        /// </summary>
        public void Clear()
        {
            // TODO: Handle special components
            foreach (var component in _list) 
                Remove(component);
        }

        /// <summary>
        /// Gets whether the <see cref="Component"/> is in <see cref="Actor"/>.
        /// </summary>
        /// <param name="item">The <see cref="Component"/> to check</param>
        /// <returns>True if <see cref="Component"/> is in <see cref="Actor"/>; otherwise, false</returns>
        public bool Contains(Component item)
        {
            return _list.Contains(item);
        }

        /// <summary>
        /// Copies <see cref="Component"/>s to an array.
        /// </summary>
        /// <param name="array">The array to be written</param>
        /// <param name="arrayIndex">The offset of array</param>
        public void CopyTo(Component[] array, int arrayIndex)
        {
            _list.CopyTo(array, arrayIndex);
        }

        /// <summary>
        /// Returns an enumerator that iterates through the <see cref="Actor"/>.
        /// </summary>
        /// <returns>An <see cref="IEnumerable{T}"/> for the <see cref="Actor"/></returns>
        public IEnumerator<Component> GetEnumerator() => _list.GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }
}