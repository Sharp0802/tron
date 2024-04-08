using System.Collections;
using GlmSharp;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Specials;

public sealed class Transform : Component
{
    private Transform? _parent;

    private static IntPtr CreateTransform(Actor actor, Transform? parent)
    {
        unsafe
        {
            var pointer = NativeMemory.Alloc<CodeGen.Transform>();
            CodeGen.Transform.__ctor__(
                pointer, 
                actor.Pointer, 
                parent is null ? null : (CodeGen.Transform*)parent.Pointer);
            return (IntPtr)pointer;
        }
    }
    
    public Transform(Actor actor, Transform? parent) : base(actor, CreateTransform(actor, parent))
    {
        _parent  = parent;
        Children = new TransformCollection(this);
    }

    public Transform? Parent
    {
        get => _parent;
        set
        {
            unsafe
            {
                CodeGen.Transform.set__Parent(
                    (CodeGen.Transform*)Pointer, 
                    value is null ? null : (CodeGen.Transform*)value.Pointer);
                if (_parent is not null)
                    Children.RemoveUnsafe(this);
                _parent = value;
            }
        }
    }
    
    public TransformCollection Children { get; }

    public vec3 Position
    {
        get
        {
            unsafe
            {
                return CodeGen.Transform.get__Position((CodeGen.Transform*)Pointer);
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Transform.set__Position((CodeGen.Transform*)Pointer, value);
            }
        }
    }

    public vec3 LocalPosition
    {
        set
        {
            unsafe
            {
                CodeGen.Transform.set__LocalPosition((CodeGen.Transform*)Pointer, value);
            }
        }
    }

    public quat Rotation
    {
        get
        {
            unsafe
            {
                return CodeGen.Transform.get__Rotation((CodeGen.Transform*)Pointer);
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Transform.set__Rotation((CodeGen.Transform*)Pointer, value);
            }
        }
    }

    public vec3 Scale
    {
        get
        {
            unsafe
            {
                return CodeGen.Transform.get__Scale((CodeGen.Transform*)Pointer);
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Transform.set__Scale((CodeGen.Transform*)Pointer, value);
            }
        }
    }

    public class TransformCollection : IReadOnlyCollection<Transform>
    {
        private readonly Transform       _transform;
        private readonly List<Transform> _children = [];

        internal TransformCollection(Transform transform)
        {
            _transform = transform;
        }
        
        public int Count
        {
            get
            {
                unsafe
                {
                    return (int)CodeGen.Transform.CountChild((CodeGen.Transform*)_transform.Pointer);
                }
            }
        }

        public void Add(Transform transform)
        {
            unsafe
            {
                CodeGen.Transform.AddChild(
                    (CodeGen.Transform*)_transform.Pointer, 
                    (CodeGen.Transform*)transform.Pointer);
                transform.Parent = _transform;
                _children.Add(transform);
            }
        }

        internal void RemoveUnsafe(Transform transform)
        {
            _children.Remove(transform);
        }
        
        public Transform this[int i] => _children[i];

        public IEnumerator<Transform> GetEnumerator()
        {
            return _children.GetEnumerator();
        }

        IEnumerator IEnumerable.      GetEnumerator()
        {
            return GetEnumerator();
        }
    }
    

    public override void OnCreated()
    {
    }

    public override void OnEnabled()
    {
    }

    public override void OnUpdated(float delta)
    {
    }

    public override void OnDisabled()
    {
    }

    public override void OnDestroyed()
    {
    }
}