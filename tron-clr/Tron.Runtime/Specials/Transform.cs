using System.Collections;
using GlmSharp;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Specials;

/// <summary>
/// A built-in <see cref="Component"/> that acts as a Transform.
/// </summary>
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

    internal Transform(Actor actor, Transform? parent) : base(actor, CreateTransform(actor, parent))
    {
        _parent  = parent;
        Children = new TransformCollection(this);
    }

    /// <summary>
    /// Gets or sets parent of this <see cref="Transform"/>.
    /// </summary>
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

    /// <summary>
    /// Gets a collection of child <see cref="Transform"/>s.
    /// </summary>
    public TransformCollection Children { get; }


    /// <summary>
    /// Gets or sets position with parent coordinate
    /// </summary>
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

    /// <summary>
    /// Sets position with local coordinate
    /// </summary>
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

    /// <summary>
    /// Gets or sets rotation with parent coordinate
    /// </summary>
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

    /// <summary>
    /// Gets or sets scale
    /// </summary>
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

    /// <summary>
    /// A collection of <see cref="Transform"/> for specific <see cref="Transform"/>.
    /// </summary>
    public class TransformCollection : IReadOnlyCollection<Transform>
    {
        private readonly Transform       _transform;
        private readonly List<Transform> _children = [];

        internal TransformCollection(Transform transform)
        {
            _transform = transform;
        }

        /// <summary>
        /// Gets the count of child <see cref="Transform"/>s
        /// </summary>
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

        /// <summary>
        /// Registers <see cref="Transform"/> as its child.
        /// </summary>
        /// <param name="transform">The <see cref="Transform"/> to set its parent to this</param>
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

        /// <summary>
        /// Gets child by index.
        /// </summary>
        /// <param name="i">The index of child</param>
        public Transform this[int i] => _children[i];

        /// <summary>
        /// Returns an enumerator that iterates through the <see cref="TransformCollection"/>.
        /// </summary>
        /// <returns></returns>
        public IEnumerator<Transform> GetEnumerator()
        {
            return _children.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }


    /// <inheritdoc />
    public override void OnCreated()
    {
    }

    /// <inheritdoc />
    public override void OnEnabled()
    {
    }

    /// <inheritdoc />
    public override void OnUpdated(float delta)
    {
    }

    /// <inheritdoc />
    public override void OnDisabled()
    {
    }

    /// <inheritdoc />
    public override void OnDestroyed()
    {
    }
}